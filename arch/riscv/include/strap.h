#ifndef _STRAP_H
#define _STRAP_H

//异常号
#define EX_ECALL_FROM_U 8
#define EX_LOAD_PF 13
#define EX_STORE_PF 15
#define EX_INSTRUCTION_PF 12
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

extern void do_timer();
#endif