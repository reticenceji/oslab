#ifndef _SCHED_H
#define _SCHED_H
#include "../../../include/types.h"

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

/* Lab6中进程的数量以及每个进程初始的时间片 */
#define LAB_TEST_NUM        1 
#define LAB_TEST_COUNTER    5  

/* 当前进程 */
extern struct task_struct *current;

/* 进程指针数组 */
extern struct task_struct * task[NR_TASKS];

/* 下面这些在Linux中放在mm.h中，line250
 * vm_flags in vm_area_struct, see mm_types.h.
 * When changing, update also include/trace/events/mmflags.h
 */
#define VM_NONE		0x00000000
#define VM_READ		0x00000001	/* currently active flags */
#define VM_WRITE	0x00000002
#define VM_EXEC		0x00000004
#define VM_SHARED	0x00000008

/* mprotect() hardcodes VM_MAYREAD >> 4 == VM_READ, and so for r/w/x bits. */
#define VM_MAYREAD	0x00000010	/* limits for mprotect() etc */
#define VM_MAYWRITE	0x00000020
#define VM_MAYEXEC	0x00000040
#define VM_MAYSHARE	0x00000080

#define PT_REGS_SIZE 0x118
/* 进程状态段数据结构 */
struct thread_struct
{
    uint64 ra;
    uint64 sp;
    uint64 s0;
    uint64 s1;
    uint64 s2;
    uint64 s3;
    uint64 s4;
    uint64 s5;
    uint64 s6;
    uint64 s7;
    uint64 s8;
    uint64 s9;
    uint64 s10;
    uint64 s11;
};
/* 进程内存管理结构 */

typedef struct { uint64 pgprot; } pgprot_t;
struct vm_area_struct {
    /* Our start address within vm_area. */
    uint64 vm_start;		
    /* The first byte after our end address within vm_area. */
    uint64 vm_end;		
    /* linked list of VM areas per task, sorted by address. */
    struct vm_area_struct *vm_next, *vm_prev;
    /* The address space we belong to. */
    struct mm_struct *vm_mm;	
    /* Access permissions of this VMA. */
    pgprot_t vm_page_prot;
    /* Flags */
    uint64 vm_flags;		
};

struct mm_struct 
{
    uint64 satp;
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
    uint64 sepc;
    uint64 sscratch;
    struct mm_struct *mm;
    struct thread_struct thread; // 该进程状态段
    uint64 *stack;  //保存异常发生时的寄存器状态
    uint64 *user_sp;
};

/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void); 

/* 在时钟中断处理中被调用 */
void do_timer(void);

/* 调度程序 */
void schedule(void);

/* 切换当前任务current到下一个任务next */
extern struct thread_struct* __switch_to(struct thread_struct* current,struct thread_struct* prev);
extern struct thread_struct* __switch_page(uint64 current_satp);
void switch_to(struct task_struct* next);

/* 死循环 */
void dead_loop(void);

/* 获取当前进程pid */
pid_t getpid(void);

/* 获得一个没有被使用过的pid */
pid_t newpid(void);

#endif

#endif
