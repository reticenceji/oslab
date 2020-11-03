#include "sched.h"
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
    task[0]->thread.sp = (unsigned long long)task[0];
    
    #ifdef SJF
    for (int i=1; i < 5; i++)
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
    for (int i=1; i < 5; i++)
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
void do_timer(void);

#ifdef SJF
/* 短作业优先式调度程序 */
void schedule(void)
{

}
#endif

#ifdef PROPRITY
/* 优先级抢占式 */
void schedule(void)
{

}
#endif

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next);

/* 死循环 */
void dead_loop(void);
