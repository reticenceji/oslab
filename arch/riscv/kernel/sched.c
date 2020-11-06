#include "sched.h"
#include "../../../include/rand.h"

#define INF 1e9

struct task_struct *current;
struct task_struct *task[NR_TASKS];

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void)
{
    current = (struct task_struct*)0x80010000;
    task[0] = current;
    task[0]->state = TASK_RUNNING;
    task[0]->counter = 0;
    task[0]->priority = 5;
    task[0]->blocked = 0;
    task[0]->pid = 0;
    task[0]->thread.sp = (unsigned long long)task[0] + TASK_SIZE;
    
    #ifdef SJF
    for (long long i=1; i < 5; i++)
    {
        task[i] = (struct task_struct*)(0x80010000 + i*TASK_SIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = rand();
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
    }
    #endif

    #ifdef PRIORITY
    for (long long i=1; i < 5; i++)
    {
        task[i] = (struct task_struct*)(0x80010000 + i*TASK_SIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 8-i;
        task[i]->priority = 5;
        task[i]->blocked = 0;
        task[i]->pid = i;
        task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
    }
    #endif
}

/* 在时钟中断处理中被调用 */
void do_timer(void)
{
    (current->counter)--;

    #ifdef SJF
    if (current->counter == 0)
    {
        schedule();
    }
    return ;
    #endif

    #ifdef PRIORITY
	schedule();
	if(current->counter == 0)//重新为该进程分配运行时长
    {
 	    for(int j = LAB_TEST_NUM; j > 0; j--)
	    {
		    if(task[j]->counter == 0)
		    {
			    task[j]->counter = 8-j;//current->counter = 8-j
		    }
	    }
    }
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
        long cnt = (*p)->counter, i = LAB_TEST_NUM;
        p = &task[LAB_TEST_NUM];
        next = 0;
        while (--i)
        {
            if (!*--p)
                continue;
            //判断是否满足运行条件
            if ((*p)->state == TASK_RUNNING && (*p)->counter > 0 && (*p)->counter <= cnt)
            {
                cnt = (*p)->counter;
                next = i;
            }
        }
        //如果cnt和next都没有被更新过，即没有符合要求的进程 (本实验中即所有counter均为0)
        //如果task[LAB_TEST_NUM-1]是应该被运行的进程，那么next会被更新，不会触发该条件
        if (cnt == task[LAB_TEST_NUM-1]->counter && next == 0)
        {
            for (int j = LAB_TEST_NUM-1; j > 0; j--)
            {
                task[j]->counter = rand();      //重新赋值
            }
        }
        else
        {
            break;
        }
    }
    #endif

    #ifdef PROPRITY
    /* 优先级抢占式 */

    #endif

    switch_to(task[next]);                      //切换到新任务
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
    __asm__(
        "sd ra,0(%1) ;\
         sd sp,8(%1) ;\
         sd s0,16(%1) ;\
         sd s1,24(%1) ;\
         sd s2,32(%1) ;\
         sd s3,40(%1) ;\
         sd s4,48(%1) ;\
         sd s5,56(%1) ;\
         sd s6,64(%1) ;\
         sd s7,72(%1) ;\
         sd s8,80(%1) ;\
         sd s9,88(%1) ;\
         sd s10,96(%1) ;\
         sd s11,104(%1) ;\
         ld ra,0(%0) ;\
         ld sp,8(%0) ;\
         ld s0,16(%0) ;\
         ld s1,24(%0) ;\
         ld s2,32(%0) ;\
         ld s3,40(%0) ;\
         ld s4,48(%0) ;\
         ld s5,56(%0) ;\
         ld s6,64(%0) ;\
         ld s7,72(%0) ;\
         ld s8,80(%0) ;\
         ld s9,88(%0) ;\
         ld s10,96(%0) ;\
         ld s11,104(%0) ;"
        :
        :"r"(& next->thread),"r"(& current->thread)
    );
}

/* 死循环 */
void dead_loop(void)
{
    while (1);
}
    
