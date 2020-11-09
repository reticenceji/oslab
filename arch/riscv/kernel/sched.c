#include "sched.h"
#include "../../../include/rand.h"
#include "put.h"
#define INF 1e9
struct task_struct *current;
struct task_struct *task[NR_TASKS];
static void init_epc();

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void)
{
    print("task init...\n");
    current = (struct task_struct*)0x80010000;
    task[0] = current;
    task[0]->state = TASK_RUNNING;
    task[0]->counter = 0;
    task[0]->priority = LAB_TEST_COUNTER;
    task[0]->blocked = 0;
    task[0]->pid = 0;
    task[0]->thread.sp = (unsigned long long)task[0] + TASK_SIZE;
    #ifdef SJF
    for (unsigned long long i=1; i <= LAB_TEST_NUM; i++)
    {
        task[i] = (struct task_struct*)(0x80010000 + i*TASK_SIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = rand();
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
        task[i]->thread.ra = (unsigned long long)init_epc;
        print("[PID = %l] Process Create Successfully! counter = %d\n",i,task[i]->counter);
    }
    init_epc();
    #endif

    #ifdef PRIORITY
    for (unsigned long long i=1; i <= LAB_TEST_NUM; i++)
    {
        task[i] = (struct task_struct*)(0x80010000 + i*TASK_SIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 8-i;
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
        task[i]->thread.ra = (unsigned long long)init_epc;
        print("[PID = %l] Process Create Successfully! counter = %d priority = %d\n",i,task[i]->counter,task[i]->priority);
    }
    init_epc();
    #endif
}

/* 在时钟中断处理中被调用 */
void do_timer(void)
{
    #ifdef SJF
    (current->counter)--;
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
    #ifdef SJF
    /* 短作业优先式调度程序 */
    struct task_struct **p;
    long next;
    while (1)
    {
    	p = &task[LAB_TEST_NUM];
        long cnt = INF, i = LAB_TEST_NUM+1;
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
                print("[PID = %d] Reset counter = %d\n",task[j]->pid,task[j]->counter);
            }
        }
        else
        {
            break;
        }
    }
    switch_to(task[next]);
    #endif

    #ifdef PRIORITY
    /* 优先级抢占式 */

    if(current != task[0])
    {
	print("tasks' priority changed\n");
        for( int i=1; i<LAB_TEST_NUM+1; i++)//重新分配task[1-4]优先级
        {
	    task[i]->priority = rand();
            print("[PID = %d] counter = %d priority = %d\n",i,task[i]->counter,task[i]->priority);
        }
            
    }
 
    (current->counter)--;
        if(current->counter <= 0)//重新为该进程分配运行时长
    {
 	    for(int j = LAB_TEST_NUM; j > 0; j--)
	    {
		    if(task[j]->counter == 0 )
		    {
			    task[j]->counter = 8-j;
                print("[PID = %d] Reset counter = %d\n",j,8-j);
		    }
	    }
    }

    struct task_struct **p;
    long next, i;
    p = &task[LAB_TEST_NUM];
    next = LAB_TEST_NUM;
    i = LAB_TEST_NUM + 1;
    long cnt1 = (*p)->priority;
    long cnt2 = (*p)->counter;
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

     switch_to(task[next]);                      //切换到新任务

     #endif
}

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next)
{
    if (next==current) return;
    /* 参考链接https://www.cnblogs.com/byeyear/p/4675049.html，更详细的查看GCC文档吧
     * __asm__(汇编语句模板: 输出部分: 输入部分: 破坏描述部分)  
     * 汇编语句模板要在一个字符串内，语句中间用分号隔开
     * %0对应& next->thread，%1对应& current->thread
     * 那个“r"的意思是寄存器类型
     * 看起来GCC没有xjb优化
     */
    struct task_struct *prev = current;
    current = next;
    __asm__(
        "sd ra,0(%1);\
         sd sp,8(%1);\
         sd s0,16(%1);\
         sd s1,24(%1);\
         sd s2,32(%1);\
         sd s3,40(%1);\
         sd s4,48(%1);\
         sd s5,56(%1);\
         sd s6,64(%1);\
         sd s7,72(%1);\
         sd s8,80(%1);\
         sd s9,88(%1);\
         sd s10,96(%1);\
         sd s11,104(%1);\
         ld ra,0(%0);\
         ld sp,8(%0);\
         ld s0,16(%0);\
         ld s1,24(%0);\
         ld s2,32(%0);\
         ld s3,40(%0);\
         ld s4,48(%0);\
         ld s5,56(%0);\
         ld s6,64(%0);\
         ld s7,72(%0);\
         ld s8,80(%0);\
         ld s9,88(%0);\
         ld s10,96(%0);\
         ld s11,104(%0);"
        :
        :"r"(& current->thread),"r"(& prev->thread)
    );
    print("[!] Switch from task %d to task %d, prio: %l, counter: %l\n",prev->pid,current->pid,current->priority,current->counter);
    return ;
}

/* 死循环 */
void dead_loop(void)
{   
    #ifdef SJF
    print("[PID = %l] Context Calculation: counter = %l\n",current->pid,current->counter);
    #endif
    while (1);
}

static void init_epc(){
    __asm__(
        "csrw sepc,%0;\
         sret;"
        :
        :"r"(dead_loop));
}
