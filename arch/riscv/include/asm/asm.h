#ifndef _ASM_RISCV_ASM_H
#define _ASM_RISCV_ASM_H

#define PT_REG_SIZE 0x118

#define PT_SEPC     0x0
#define PT_STVAL    0x8
#define PT_SCAUSE   0x10
#define PT_SSTATUS  0x18
#define PT_MEPC     0x0
#define PT_MTVAL    0x8
#define PT_MCAUSE   0x10
#define PT_MSTATUS  0x18

#define PT_S11      0x40
#define PT_S10      0x48
#define PT_S9       0x50
#define PT_S8       0x58
#define PT_S7       0x60
#define PT_S6       0x68
#define PT_S5       0x70
#define PT_S4       0x78
#define PT_S3       0x80
#define PT_S2       0x88
#define PT_S1       0xd0
#define PT_S0       0xd8

#define PT_T6       0x20
#define PT_T5       0x28
#define PT_T4       0x30
#define PT_T3       0x38
#define PT_T2       0xe0
#define PT_T1       0xe8
#define PT_T0       0xf0

#define PT_A7       0x90
#define PT_A6       0x98
#define PT_A5       0xa0
#define PT_A4       0xa8
#define PT_A3       0xb0
#define PT_A2       0xb8
#define PT_A1       0xc0
#define PT_A0       0xc8

#define PT_TP       0xf8
#define PT_GP       0x100
#define PT_RA       0x110

#define MTIME       0x200bff8
#define MTIMECMP    0x2004000
#define MAP_OFFSET  0xffffffe000000000-0x80000000

#define SCAUSE_STI_NUM  0x8000000000000005
#define CODE_MASK   0xFF 	
#endif