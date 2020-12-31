#include "put.h"
#include "syscall.h"
#include "strap.h"

void handler_s(size_t scause, size_t sepc, uintptr_t *regs)
{
    //可能会有两个返回值，虽然目前都只有一个
    unsigned long long ret0=0,ret1=0;
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
        default:
            break;
        }
        regs[REG_A0] = ret0;    //返回值通过a0传递
        break;
    case EX_INSTRUCTION_PF:
        print("instruction page fault at %X\n",sepc);
        while (1);
        break;
    case EX_LOAD_PF:
        print("store page fault at %X\n",sepc);
        break;
    case EX_STORE_PF:
        print("store page fault at %X\n",sepc);
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
