#include "put.h"
#include "syscall.h"
#include "strap.h"
#include "sched.h"
#include "../../../include/string.h"

void handler_s(size_t scause, size_t sepc, uintptr_t *regs)
{
    //可能会有两个返回值，虽然目前都只有一个
    uint64 ret0=0,ret1=0;
    switch (scause)
    {
    case EX_ECALL_FROM_U:
        switch (regs[REG_A7])   //A7是系统调用号
        {
        case SYS_GETPID:
            ret0 = sys_getpid();
            break;
        case SYS_WRITE:
            ret0 = sys_write(regs[REG_A0],(const char*)regs[REG_A1],regs[REG_A2]);
            break;
        case SYS_MUNMAP:
            ret0 = munmap ((void *) regs[REG_A0], (size_t) REG_A1);
            break;
        case SYS_FORK:
            memmove(current->stack,regs,PT_REGS_SIZE);
            ret0 = (uint64)fork();
            break;
        case SYS_MMAP:
            ret0 = (uint64)mmap ((void *)regs[REG_A0], (size_t)regs[REG_A1], (int) regs[REG_A2],
                   (int)regs[REG_A3], (int)regs[REG_A4] , (__off_t) regs[REG_A5]);
            break;
        case SYS_MPROTECT:
            ret0 = mprotect ((void *) regs[REG_A0], (size_t) regs[REG_A1], (int) regs[REG_A2]);
            break;
        default:
            break;
        }
        regs[REG_A0] = ret0;    //返回值通过a0传递
        break;
    case EX_INSTRUCTION_PF:
    case EX_LOAD_PF:
    case EX_STORE_PF:
        do_page_fault(regs);
        break;
    default:
        break;
    }
    sepc += 4;
    regs[CSR_SEPC] = sepc;
    return;
}

void sti_handler_s()
{
	static int count=0,if_hundred=0;
	if (if_hundred==100)	
	{
		if_hundred=0;
		do_timer();
	}
	if_hundred++;

	return;
}
