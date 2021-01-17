#include "syscall.h"
#include "vma.h"
#include "vm.h"
#include "sched.h"
#include "strap.h"

void do_page_fault(uintptr_t *regs)
{
    uint64 stval,scause,sepc,sp;
    struct vm_area_struct* vma;
    stval = regs[CSR_STVAL];
    scause = regs[CSR_SCAUSE];
    sepc = regs[CSR_SEPC];
    vma = vma_find(stval);
    print("[S] PAGE_FAULT: PID = %d\n",current->pid);
    print("[S] PAGE_FAULT: scause: %d, sepc: 0x%X, badaddr: 0x%X\n",scause,sepc,stval);
    // 如果VM没有被映射，错误
    if (vma==NULL) 
    {
        print("Invalid vm area in page \n");
        return;
    }
    if (scause == EX_LOAD_PF && vma->vm_flags & VM_READ) 
    {
        //在我们的实验中，读写错误就给他分配一块frame写，kernel和user统一用kmalloc分配
        if (current->pid >1) 
        {
            sp = kmalloc(PAGE_SIZE);
            create_mapping(current->mm->satp, stval & ~PAGE_MASK, sp, PAGE_SIZE, FLAG_R|FLAG_V|FLAG_U);
        }
        else
            create_mapping(current->mm->satp, stval & ~PAGE_MASK, current->user_sp, PAGE_SIZE, FLAG_R|FLAG_V|FLAG_U);
        return;
    }
    if (scause == EX_STORE_PF && vma->vm_flags & VM_WRITE) 
    {
        //在我们的实验中，读写错误就给他分配一块frame写，kernel和user统一用kmalloc分配
        if (current->pid >1) 
        {
            sp = kmalloc(PAGE_SIZE);
            create_mapping(current->mm->satp, stval & ~PAGE_MASK, sp, PAGE_SIZE, FLAG_R|FLAG_W|FLAG_V|FLAG_U);
        }
        else
            create_mapping(current->mm->satp, stval & ~PAGE_MASK, current->user_sp, PAGE_SIZE, FLAG_R|FLAG_W|FLAG_V|FLAG_U);
        return;
    }
    if (scause == EX_INSTRUCTION_PF && vma->vm_flags & VM_EXEC) 
    {
        //在我们的实验中，执行错误就把它映射到程序装载的地方
        create_mapping(current->mm->satp, stval & ~PAGE_MASK, 0x84000000, PAGE_SIZE, FLAG_R|FLAG_X|FLAG_V|FLAG_U);
        return;
    }
    print("Invalid vm area in page fault\n");
    return;
}