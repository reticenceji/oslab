#include "sched.h"
#include "../../../include/types.h"
#include "slub.h"


extern void ret_from_fork(uint64 *stack);
/* TODO 为子进程创建它的task_struct(包括Stack的整个Page，并修改部分内容。如果创建失败（NR_TASK）返回NULL，否则返回它的地址 
 * stack内容
 */

/* TODO 主要功能是调用汇编的ret_from_fork */
void fork_ret()
{
    ret_from_fork(current->stack);
    return;
}
void copy_mm(struct task_struct *child)
{
    child->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));
    child->mm->satp = kmalloc(PAGE_SIZE); //新开页表
    kernel_mapping(child->mm->satp);    //映射kernel部分，按理来说是复制父进程的？这样子没有考虑kernel部分的映射发生改变的情况，也就是第一个TA说的。
    copy_mmap(child);//复制
}

void copy_mmap(struct task_struct *child)
{
    vma_copy(child->mm);
}

struct task_struct* dup_task_struct (struct task_struct *current)
{
    struct task_struct *child = (struct task_struct *)kmalloc(TASK_SIZE);
    memmove(child,current,TASK_SIZE);   //string.h里给的，能用吗，还是要自己写一个？
    child->pid = new_pid();//可以给pid创建一个bitmap标记使用
    child->user_sp = kmalloc(PAGE_SIZE);  //user stack
    memmove(child->user_sp,current->stack,PAGE_SIZE);
    child->thread.sp = (uint64)child+PAGE_SIZE; //kernel stack
    child->thread.ra = fork_ret;
    return child;
}

/* TODO 对于父进程，返回子进程的PID；对于子进程，返回0 */
pid_t fork()
{
    struct task_struct *child_ts;
    child_ts =  dup_task_struct(current);
    copy_mm(child_ts);
    /* 复制用户空间的内容
     * 代码段、数据段不用，写死在0x84000000，不管他? 这个需要吗？
     * 堆栈需要另外开一段，并且复制parent堆栈的内容。并且我们需要保存他在task——struct的某个地方。
     * 创建页表
     */
    return child_ts->pid;
}


