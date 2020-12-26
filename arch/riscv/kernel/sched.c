#include "sched.h"
#include "vm.h"
#include "../../../include/rand.h"
#include "put.h"
#define INF 1e9
struct task_struct *current;
struct task_struct *task[NR_TASKS];
static void init_epc();

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void)
{
    // print("%X\n",task_init);
    #ifdef DEBUGSTORETEXT
        int *x=(int *)task_init;
        *x = 1;
    #endif
    #ifdef DEBUGSTORERODATA
        int *x=(int *)"\x97\x32\x00\x00";
        *x = 1;
    #endif
    #ifdef DEBUGEXERODATA
    int (*x) (int)=(int (*)(int))"\x97\x32\x00\x00";
    int a=(*x)(5);
    #endif
    #ifdef DEBUGEXESTACK
    char ins[] = "\x97\x32\x00\x00";
    int (*x) (int)=(int (*)(int))ins;
    int a=(*x)(5);
    #endif
    #ifdef DEBUGEXEDATA
    static char ins[] = "\x97\x32\x00\x00";
    int (*x) (int)=(int (*)(int))ins;
    int a=(*x)(5);
    #endif

    print("task init...\n");
    current = (struct task_struct*)(KERNEL_TASK_START_V);
    task[0] = current;
    task[0]->state = TASK_RUNNING;
    task[0]->counter = 0;
    task[0]->priority = 5;
    task[0]->blocked = 0;
    task[0]->pid = 0;
    task[0]->thread.sp = (unsigned long long)task[0] + TASK_SIZE;
    #ifdef SJF
    for (unsigned long long i=1; i <= LAB_TEST_NUM; i++)
    {
        unsigned long long pgtbl = alloc_frame();

        task[i] = (struct task_struct*)(KERNEL_TASK_START_V + i*TASK_SIZE);

        task[i]->mm->satp = pgtbl;
        create_mapping((uint64*)pgtbl, USER_TASK_START_V, USER_TASK_START_P, USER_TASK_SIZE,  FLAG_U|FLAG_R|FLAG_W|FLAG_X|FLAG_V);
        create_mapping((uint64*)pgtbl, USER_STACK_TOP_V,  USER_STACK_TOP_P,  USER_STACK_SIZE, FLAG_U|FLAG_R|FLAG_W|FLAG_V);
        kernel_mapping((uint64*)pgtbl);

        task[i]->state = TASK_RUNNING;
        task[i]->counter = rand();
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;

        task[i]->thread.sp = USER_TASK_SIZE;
        task[i]->sscratch = (unsigned long long)task[i] + TASK_SIZE;    //我的理解是thread.sp是用户栈, sscratch是内核栈, 切换时从这里取值交换
        task[i]->thread.ra = (unsigned long long)init_epc;
        print("[PID = %l] Process Create Successfully! counter = %d\n",i,task[i]->counter);
    }
    init_epc();
    #endif

    #ifdef PRIORITY
    for (unsigned long long i=1; i <= LAB_TEST_NUM; i++)
    {        
        unsigned long long pgtbl = alloc_frame();

        task[i] = (struct task_struct*)(KERNEL_TASK_START_V + i*TASK_SIZE);

        task[i]->mm->satp = pgtbl;
        create_mapping((uint64*)pgtbl, USER_TASK_START_V, USER_TASK_START_P, USER_TASK_SIZE,  FLAG_U|FLAG_R|FLAG_W|FLAG_X|FLAG_V);
        create_mapping((uint64*)pgtbl, USER_STACK_TOP_V,  USER_STACK_TOP_P,  USER_STACK_SIZE, FLAG_U|FLAG_R|FLAG_W|FLAG_V);
        kernel_mapping((uint64*)pgtbl);

        task[i]->state = TASK_RUNNING;
        task[i]->counter = 7-(i-1)%4;
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;

        task[i]->thread.sp = USER_TASK_SIZE;
        task[i]->sscratch = (unsigned long long)task[i] + TASK_SIZE;
        task[i]->thread.ra = (unsigned long long)init_epc;
        print("[PID = %l] Process Create Successfully! counter = %d priority = %d\n",i,task[i]->counter,task[i]->priority);
    }
    init_epc();
    #endif
}

/* 在时钟中断处理中被调用 */
void do_timer(void)
{
    (current->counter)--;
    #ifdef SJF

    if (current->counter <= 0)
    {
        schedule();
    }
    print("[PID = %l] Context Calculation: counter = %l\n",current->pid,current->counter);
    return ;
    #endif

    #ifdef PRIORITY
    schedule();
    return ;
    #endif
}

void schedule(void)
{
    /* 短作业优先式调度程序 */
    #ifdef SJF
    struct task_struct **p;
    long next;
    while (1)
    {
    	p = &task[NR_TASKS-1];
        long cnt = INF, i = NR_TASKS;
        next = 0;
        while (--i)
        {
            if (!(*p))
            {
                --p;
                continue;
            }
            //判断是否满足运行条件
            if ((*p)->state == TASK_RUNNING && (*p)->counter > 0 && (*p)->counter < cnt)
            {
                cnt = (*p)->counter;
                next = i;
            }
            --p;
        }
        //如果cnt和next都没有被更新过，即没有符合要求的进程 (本实验中即所有counter均为0)
        //如果task[LAB_TEST_NUM]是应该被运行的进程，那么next会被更新，不会触发该条件
        if (cnt == INF && next == 0)
        {
            for (int j = 1; j <= LAB_TEST_NUM; j++)
            {
                task[j]->counter = rand();      //重新赋值
                print("[PID = %l] Reset counter = %l\n",task[j]->pid,task[j]->counter);
            }
        }
        else
        {
            break;
        }
    }
    if (current!=task[next])
        print("[!] Switch from task %l [task struct:0x%X, sp:0x%X] to task %l [task struct:0x%X, sp:0x%X], prio: %l, counter: %l\n", current->pid, current, current->thread.sp ,task[next]->pid, task[next], task[next]->thread.sp,task[next]->priority, task[next]->counter);
    switch_to(task[next]);
    #endif

    /* 优先级抢占式调度程序 */
    #ifdef PRIORITY
    if(current->counter <= 0)//重新为该进程分配运行时长
    {
 	    for(int j = NR_TASKS-1; j > 0; j--) 
	    {
		    if(task[j] != 0 && task[j]->counter == 0)//&& current == task[j]
		    {
			    task[j]->counter = 7-(j-1)%4;//current->counter = 7-(j-1)%4
                print("[PID = %l] Reset counter = %l\n", task[j]->pid, 7-(j-1)%4);
		    }
	    }
    }
    struct task_struct **p;
    long next, i;
    p = &task[NR_TASKS-1];
    next = NR_TASKS-1;
    i = NR_TASKS;
    long cnt1 = INF;
    long cnt2 = INF;
    while (--i)
    {
        if (!(*p))
        {
            --p;
            continue;
        }
        //判断是否满足运行条件
        if ((*p)->state == TASK_RUNNING && (*p)->counter > 0 && ((*p)->priority < cnt1 || ((*p)->priority == cnt1 && (*p)->counter < cnt2)))
        {
            cnt1 = (*p)->priority;
            cnt2 = (*p)->counter;
            next = i;
        }
        --p;
    }

	if(current != task[next]){
        print("[!] Switch from task %l [task struct:0x%X, sp:0x%X] to task %l [task struct:0x%X, sp:0x%X], prio: %l, counter: %l\n", current->pid, current, current->thread.sp ,task[next]->pid, task[next], task[next]->thread.sp,task[next]->priority, task[next]->counter);
}

	print("tasks' priority changed\n");
    for( int i=1; i < NR_TASKS && task[i] != 0; i++)//重新分配task[1-4]优先级
    {
        task[i]->priority = rand();
        print("[PID = %l] counter = %l priority = %l\n",task[i]->pid,task[i]->counter,task[i]->priority);    
    }
    switch_to(task[next]);                      //切换到新任务
    #endif
}

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next)
{
    if (next==current) return;
    register struct task_struct *prev = current;
    current = next;
    __switch_to(& current->thread,& prev->thread);
}

/* 死循环 */
void dead_loop(void)
{
    #ifdef SJF
    print("[PID = %l] Context Calculation: counter = %l\n",current->pid,current->counter);
    #endif
    while (1);
}

static void init_epc()
{
    __asm__ __volatile__(
        "li t1,0x22;\
         csrw sstatus,t1;\
         csrw sepc,0x0;\
         sret;"
         );
}

long getpid()
{
    return current->pid;
}
