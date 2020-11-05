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
    return ;
}

/* 死循环 */
void dead_loop(void);
