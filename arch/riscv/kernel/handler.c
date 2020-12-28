#include "put.h"
#include "syscall.h"
//异常号
#define EX_ECALL_FROM_U 8
#define EX_LOAD_PF 13
#define EX_STORE_PF 15
#define EX_INSTRUCTION_PF 12
//系统调用号
#define SYS_WRITE 64
#define SYS_GETPID 172
//用来在stack上寻址
#define REG_A0 25
#define REG_A1 24
#define REG_A2 23
#define REG_A3 22
#define REG_A4 21
#define REG_A5 20
#define REG_A6 19
#define REG_A7 18
#define CSR_SEPC 0


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
