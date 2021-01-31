#include "sched.h"
#include "string.h"
#include "slub.h"
#include "syscall.h"
#include "vma.h"
#include "vm.h"

extern void ret_from_fork(uint64 *stack,uint64* kernel_sp);
static void dup_mm(struct task_struct *child);
static struct task_struct* dup_task_struct (struct task_struct *current);

/* 为子进程创建它的task_struct(包括Stack的整个Page，并修改部分内容。
 * stack内容
 * TODO kernel部分映射发生改变的情况没有考虑
 */
static void dup_mm(struct task_struct *child)
{
    uint64 pgtbl;
    child->mm = (struct mm_struct *)kmalloc(sizeof(struct mm_struct));

    pgtbl = kmalloc(PAGE_SIZE);
    child->mm->satp = PGTBL2SATP(PP(pgtbl)); //新开页表
    kernel_mapping((uint64 *)pgtbl);    //映射kernel部分，按理来说是复制父进程的？这样子没有考虑kernel部分的映射发生改变的情况，也就是第一个TA说的。
    
    vma_copy(child->mm);
}

static struct task_struct* dup_task_struct (struct task_struct *current)
{
    struct task_struct *child = (struct task_struct *)kmalloc(TASK_SIZE);
    memmove(child,current,TASK_SIZE);   
    child->pid = newpid();
    child->user_sp = kmalloc(PAGE_SIZE);  //user stack PA
    memmove(child->user_sp,USER_STACK_TOP_V,PAGE_SIZE);
    child->thread.sp = (uint64)child+PAGE_SIZE; //kernel stack
    child->sscratch = __get_sscratch();     //user stack VA
    child->thread.ra = (uint64)fork_ret;
    child->stack = kmalloc(PT_REGS_SIZE);
    memmove(child->stack,current->stack,PT_REGS_SIZE);
    return child;
}

/* output:对于父进程，返回子进程的PID；对于子进程，返回0，不过不是在此处返回。
 * 要完整的拷贝父进程的内容，以及修改必要的内容
 */
pid_t fork()
{
    struct task_struct *child_ts;
    child_ts =  dup_task_struct(current);
    dup_mm(child_ts);
    task[child_ts->pid] = child_ts;
    print("[PID = %d] Process fork from [PID = %d] Successfully! counter = %d\n",child_ts->pid,current->pid,current->counter);
    return child_ts->pid;
}

/* 调用汇编的ret_from_fork */
void fork_ret()
{
    ret_from_fork(current->stack,current->thread.sp);
    return;
}
