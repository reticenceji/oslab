#include "sched.h"

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void); 

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