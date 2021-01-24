#include "sched.h"
#include "../../../include/types.h"
#include "../../../include/string.h"
#include "slub.h"
#include "syscall.h"
#include "vma.h"
#include "vm.h"
static void copy_mm(struct task_struct *child);
static void copy_mmap(struct task_struct *child);
static struct task_struct* dup_task_struct (struct task_struct *current);
extern void ret_from_fork(uint64 *stack);

/* TODO 为子进程创建它的task_struct(包括Stack的整个Page，并修改部分内容。如果创建失败（NR_TASK）返回NULL，否则返回它的地址 
 * stack内容
 */
static void copy_mm(struct task_struct *child)
{
    uint64 pgtbl;
    child->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));
    pgtbl = kmalloc(PAGE_SIZE);
    child->mm->satp = (PP(pgtbl)>>12)|MODE_SV39; //新开页表
    kernel_mapping((uint64 *)pgtbl);    //映射kernel部分，按理来说是复制父进程的？这样子没有考虑kernel部分的映射发生改变的情况，也就是第一个TA说的。
    //copy_mmap(child);//复制
    vma_copy(child->mm);
}

static struct task_struct* dup_task_struct (struct task_struct *current)
{
    struct task_struct *child = (struct task_struct *)kmalloc(TASK_SIZE);
    memmove(child,current,TASK_SIZE);   //string.h里给的，能用吗，还是要自己写一个？
    child->pid = newpid();//可以给pid创建一个bitmap标记使用
    child->user_sp = kmalloc(PAGE_SIZE);  //user stack
    memmove(child->user_sp,USER_STACK_TOP_V,PAGE_SIZE);
    child->thread.sp = (uint64)child+PAGE_SIZE; //kernel stack
    child->sscratch = __get_sscratch();
    child->thread.ra = (uint64)fork_ret;
    return child;
}

/* TODO 对于父进程，返回子进程的PID；对于子进程，返回0 */
pid_t fork()
{
    struct task_struct *child_ts;
    child_ts =  dup_task_struct(current);
    copy_mm(child_ts);
    child_ts->stack = kmalloc(PT_REGS_SIZE);
    memmove(child_ts->stack,current->stack,PT_REGS_SIZE);
    /* 复制用户空间的内容
     * 代码段、数据段不用，写死在0x84000000，不管他? 这个需要吗？
     * 堆栈需要另外开一段，并且复制parent堆栈的内容。并且我们需要保存他在task——struct的某个地方。
     * 创建页表
     */
    task[child_ts->pid] = child_ts;
    print("[PID = %d] Process fork from [PID = %d] Successfully! counter = %d\n",child_ts->pid,current->pid,current->counter);
    return child_ts->pid;
}

/* TODO 主要功能是调用汇编的ret_from_fork */
void fork_ret()
{
    ret_from_fork(current->stack);
    return;
}
