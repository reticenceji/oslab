#ifndef _SCHED_H
#define _SCHED_H

#define TASK_SIZE   (4096)
#define THREAD_OFFSET  (5 * 0x08)

#ifndef __ASSEMBLER__

/* task的最大数量 */
#define NR_TASKS    64

#define FIRST_TASK  (task[0])
#define LAST_TASK   (task[NR_TASKS-1])

/* 定义task的状态，Lab3中task只需要一种状态。*/
#define TASK_RUNNING                0
// #define TASK_INTERRUPTIBLE       1
// #define TASK_UNINTERRUPTIBLE     2
// #define TASK_ZOMBIE              3
// #define TASK_STOPPED             4

#define PREEMPT_ENABLE  0
#define PREEMPT_DISABLE 1

/* Lab3中进程的数量以及每个进程初始的时间片 */
#define LAB_TEST_NUM        4 
#define LAB_TEST_COUNTER    5  

/* 当前进程 */
extern struct task_struct *current;

/* 进程指针数组 */
extern struct task_struct * task[NR_TASKS];

/* 进程状态段数据结构 */
struct thread_struct
{
    unsigned long long ra;
    unsigned long long sp;
    unsigned long long s0;
    unsigned long long s1;
    unsigned long long s2;
    unsigned long long s3;
    unsigned long long s4;
    unsigned long long s5;
    unsigned long long s6;
    unsigned long long s7;
    unsigned long long s8;
    unsigned long long s9;
    unsigned long long s10;
    unsigned long long s11;
};
/* 进程内存管理结构 */

typedef struct { unsigned long pgprot; } pgprot_t;
struct vm_area_struct {
    /* Our start address within vm_area. */
    unsigned long vm_start;		
    /* The first byte after our end address within vm_area. */
    unsigned long vm_end;		
    /* linked list of VM areas per task, sorted by address. */
    struct vm_area_struct *vm_next, *vm_prev;
    /* The address space we belong to. */
    struct mm_struct *vm_mm;	
    /* Access permissions of this VMA. */
    pgprot_t vm_page_prot;
    /* Flags*/
    unsigned long vm_flags;		
    /* next vma */
    struct vm_area_struct *next;
    /* prev vma */
    struct vm_area_struct *prev;
};

struct mm_struct 
{
    unsigned long long satp;
    struct vm_area_struct *mmap;    //参照Linux实际实现命名
};
/* 进程数据结构 */
struct task_struct
{
     long state;    // 进程状态 Lab3中进程初始化时置为TASK_RUNNING
     long counter;  // 运行剩余时间 
     long priority; // 运行优先级 1最高 5最低
     long blocked;
     long pid;      // 进程标识符
    // Above Size Cost: 40 bytes

    //TODO add sepc, sscratch, mm_struct *mm etc. here
    unsigned long long sepc;
    unsigned long long sscratch;
    struct mm_struct *mm;
    struct thread_struct thread; // 该进程状态段
};

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void); 

/* 在时钟中断处理中被调用 */
void do_timer(void);

/* 调度程序 */
void schedule(void);

/* 切换当前任务current到下一个任务next */
extern struct thread_struct* __switch_to(struct thread_struct* current,struct thread_struct* prev);
extern struct thread_struct* __switch_page(unsigned long long current_satp);
void switch_to(struct task_struct* next);

/* 死循环 */
void dead_loop(void);

/* 获取当前进程pid */
long getpid(void);

#endif

#endif
