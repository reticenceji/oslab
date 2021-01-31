#include "asm/asm.h"

.section .text.init
.globl _start
_start:
    la t0, _bss_start
    la t1, _bss_end             #后面那个符号是lds给出的
loop_clear_bss:
    beq t0, t1, done_clear_bss
    sb zero, 0(t0) 
    addi t0, t0, 1
    j loop_clear_bss 
done_clear_bss:                 #循环的将.bss段内容设置为0

    li t0, MTIME
    ld t0, 0(t0)
    li t1, 1000000
    add t0, t0, t1
    li t1, MTIMECMP
    sd t0, 0(t1)                #设置mtimecmp = mtime+1000000
    
    li t0, 0x20
    csrw mideleg, t0            #设置委托，时钟中断
    li t0, 0xB100
    csrw medeleg, t0            #设置委托，instruction/load/store page fault
    li t0, 0xA0
    csrw mie, t0                #mie[mtie] = 1, sie[stie] = 1, 即允许时钟中断

    li t0, 0x7FF
    addi t0, t0, 0x12B
    csrw mstatus, t0            #mstatus[mpp] = mstatus[mie] = mstatus[sie] = 1

    la t0, _start_retaddr
    csrw mepc, t0               #设置mret的返回地址
    la t0, exception_addr_m
    csrw mtvec, t0              #设置machine mode异常处理地址
    la sp, stack_top
    csrw mscratch, sp           #设置M mode的堆栈
    mret

_start_retaddr:
    la t0, virtual_next
    li t1, MAP_OFFSET
    add t0,t0,t1
    csrw stvec, t0                  #设置supervisor mode异常处理地址
    call init_buddy_system
    call paging_init

    call get_kernel_satp
    csrw satp,a0
virtual_next:
    sfence.vma zero, zero

    li t0,0x40122
    csrw sstatus,t0                 
    csrw scause,zero                #我们利用instruciton page fault来到这里,应该要把他置位回去
    la t0, exception_addr_s 
    csrw stvec, t0    #set s mode interupt handle place

    la sp, init_stack_top
    csrw sscratch, sp
    call slub_init
    
    j start_kernel

.align 2
.globl exception_addr_m
exception_addr_m:
    csrrw sp, mscratch, sp        #交换sp和mscratch的值
    addi sp,sp,-PT_REG_SIZE
    sd s0,PT_S0(sp)
    sd s1,PT_S1(sp)
    sd s2,PT_S2(sp)
    sd s3,PT_S3(sp)
    sd s4,PT_S4(sp)
    sd s5,PT_S5(sp)
    sd s6,PT_S6(sp)
    sd s7,PT_S7(sp)
    sd s8,PT_S8(sp)
    sd s9,PT_S9(sp)
    sd s10,PT_S10(sp)
    sd s11,PT_S11(sp)
    sd a0,PT_A0(sp)
    sd a1,PT_A1(sp)
    sd a2,PT_A2(sp)
    sd a3,PT_A3(sp)
    sd a4,PT_A4(sp)
    sd a5,PT_A5(sp)
    sd a6,PT_A6(sp)
    sd a7,PT_A7(sp)
    sd t0,PT_T0(sp)
    sd t1,PT_T1(sp)
    sd t2,PT_T2(sp)
    sd t3,PT_T3(sp)
    sd t4,PT_T4(sp)
    sd t5,PT_T5(sp)
    sd t6,PT_T6(sp)
    sd tp,PT_TP(sp)
    sd gp,PT_GP(sp)
    sd ra,PT_RA(sp)
    csrr t0, mstatus
    sd t0,PT_MSTATUS(sp)
    csrr t0, mcause
    sd t0,PT_MCAUSE(sp)
    csrr t0, mtval
    sd t0,PT_MTVAL(sp)
    csrr t0, mepc
    sd t0,PT_MEPC(sp)

    csrr t0, mcause
    blt t0,zero,interupt_table_m	#如果最高位是0，那么是中断。
exception_table_m:
ecall_handler_m:            #I think the most optimum way is set mtvec[low]=1,use jump table
    li t1, MTIMECMP
    ld t1, 0(t1)
    li t0, 100000
    add t1, t1, t0
    li t0, MTIMECMP
    sd t1, 0(t0)                    #mtimecmp += 100000
    li t0, 0x80
    csrs mie, t0                    #mie[mtie] = 1
    li t0, 0x20
    csrc mip,t0
    ld t0,PT_MEPC(sp)
    addi t0, t0, 4
    sd t0,PT_MEPC(sp)
    j exception_retaddr_m

interupt_table_m:
mti_handler_m:
    li t0, 0x80
    csrc mie, t0                    #mie[mtie] = 0
    li t0, 0x20
    csrs mip, t0                    #mip[stip] = 1
    j exception_retaddr_m

exception_retaddr_m:
    ld t0,PT_MEPC(sp)
    csrw mepc, t0
    ld t0,PT_MTVAL(sp)
    csrw mtval, t0 
    ld t0,PT_MCAUSE(sp)
    csrw mcause,t0
    ld t0,PT_MSTATUS(sp)
    csrw mstatus, t0
    ld s0,PT_S0(sp)
    ld s1,PT_S1(sp)
    ld s2,PT_S2(sp)
    ld s3,PT_S3(sp)
    ld s4,PT_S4(sp)
    ld s5,PT_S5(sp)
    ld s6,PT_S6(sp)
    ld s7,PT_S7(sp)
    ld s8,PT_S8(sp)
    ld s9,PT_S9(sp)
    ld s10,PT_S10(sp)
    ld s11,PT_S11(sp)
    ld a0,PT_A0(sp)
    ld a1,PT_A1(sp)
    ld a2,PT_A2(sp)
    ld a3,PT_A3(sp)
    ld a4,PT_A4(sp)
    ld a5,PT_A5(sp)
    ld a6,PT_A6(sp)
    ld a7,PT_A7(sp)
    ld t0,PT_T0(sp)
    ld t1,PT_T1(sp)
    ld t2,PT_T2(sp)
    ld t3,PT_T3(sp)
    ld t4,PT_T4(sp)
    ld t5,PT_T5(sp)
    ld t6,PT_T6(sp)
    ld tp,PT_TP(sp)
    ld gp,PT_GP(sp)
    ld ra,PT_RA(sp)
    addi sp,sp,PT_REG_SIZE
    csrrw sp,mscratch,sp    
    mret