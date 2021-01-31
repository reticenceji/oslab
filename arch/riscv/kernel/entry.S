#include "asm/asm.h"

.globl exception_addr_s
.section .text.entry
exception_addr_s:
    //sscratch存的是kernel stack的地址，和Linux的实现方式有一定区别
    csrrw sp,sscratch,sp
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
    csrr t0, sstatus
    sd t0,PT_SSTATUS(sp)
    csrr t0, scause
    sd t0,PT_SCAUSE(sp)
    csrr t0, stval
    sd t0,PT_STVAL(sp)
    csrr t0, sepc
    sd t0,PT_SEPC(sp)
    
    csrr t0,scause
    blt t0,zero,interupt_table_s		#如果最高位是0，那么是中断。
    csrr a0,scause		#第一个参数 scause
    csrr a1,sepc		#第二个参数 sepc
    mv a2,sp			#第三个参数 regs
    call handler_s		#所有的异常都在这里处理
    j exception_retaddr_s

interupt_table_s:
    li t1,SCAUSE_STI_NUM			#对应于Supervisor timer interrupt
    bne t0,t1,other_interupt_s
    call sti_handler_s
    ecall								#产生ecall_from_s_mode exception，重新设置mtimecmp
    j exception_retaddr_s
other_interupt_s:
    j exception_retaddr_s	

exception_retaddr_s:							
    #恢复所有的寄存器
    ld t0,PT_SEPC(sp)
    csrw sepc, t0
    ld t0,PT_STVAL(sp)
    csrw stval, t0 
    ld t0,PT_SCAUSE(sp)
    csrw scause,t0
    ld t0,PT_SSTATUS(sp)
    csrw sstatus, t0
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
    csrrw sp,sscratch,sp
    sret

/* input: a0=current.mm.satp
 * output: no
 * 交换satp
 */
.globl __switch_page
.type __switch_page,@function
__switch_page:
    csrw satp, a0
    sfence.vma zero, zero
    ret
	
/* input: a0=current,a1=prev
 * output: no
 * 交换thread_info
 */
.globl __switch_to
.type __switch_to,@function
__switch_to:
    csrr t0,sscratch
    sd t0,-16(a1)
    sd ra,0(a1)
    sd sp,8(a1)
    sd s0,16(a1)
    sd s1,24(a1)
    sd s2,32(a1)
    sd s3,40(a1)
    sd s4,48(a1)
    sd s5,56(a1)
    sd s6,64(a1)
    sd s7,72(a1)
    sd s8,80(a1)
    sd s9,88(a1)
    sd s10,96(a1)
    sd s11,104(a1)

    ld t0,-16(a0)
    csrw sscratch,t0
    ld ra,0(a0)
    ld sp,8(a0)
    ld s0,16(a0)
    ld s1,24(a0)
    ld s2,32(a0)
    ld s3,40(a0)
    ld s4,48(a0)
    ld s5,56(a0)
    ld s6,64(a0)
    ld s7,72(a0)
    ld s8,80(a0)
    ld s9,88(a0)
    ld s10,96(a0)
    ld s11,104(a0)
    ret

/* input: no
 * output: 如果页表未开启，返回0
 */ 
.globl __is_page_open
.type __is_page_open,@function
__is_page_open:
    csrr a0,satp
    srli a0,a0,60 
    ret

.globl __get_sscratch
.type __get_sscratch,@function
__get_sscratch:
    csrr a0,sscratch
    ret
/* input:   a0 = stack
 *          a1 = kernel_sp
 * output:  a0 = 0
 * fork的返回，从事先保存的寄存器中把寄存器读出来
 */
.globl ret_from_fork
.type ret_from_fork,@function
ret_from_fork:
    csrr sp,sscratch		#继承父进程的用户栈虚拟地址
    csrw sscratch,a1		#和父进程有不一样的系统栈地址

    ld t0,PT_SEPC(a0)
    addi t0,t0,4
    csrw sepc, t0
    ld t0,PT_STVAL(a0)
    csrw stval, t0 
    ld t0,PT_SCAUSE(a0)
    csrw scause,t0
    ld t0,PT_SSTATUS(a0)
    csrw sstatus, t0
    ld s0,PT_S0(a0)
    ld s1,PT_S1(a0)
    ld s2,PT_S2(a0)
    ld s3,PT_S3(a0)
    ld s4,PT_S4(a0)
    ld s5,PT_S5(a0)
    ld s6,PT_S6(a0)
    ld s7,PT_S7(a0)
    ld s8,PT_S8(a0)
    ld s9,PT_S9(a0)
    ld s10,PT_S10(a0)
    ld s11,PT_S11(a0)
    ld a1,PT_A1(a0)
    ld a2,PT_A2(a0)
    ld a3,PT_A3(a0)
    ld a4,PT_A4(a0)
    ld a5,PT_A5(a0)
    ld a6,PT_A6(a0)
    ld a7,PT_A7(a0)
    ld t0,PT_T0(a0)
    ld t1,PT_T1(a0)
    ld t2,PT_T2(a0)
    ld t3,PT_T3(a0)
    ld t4,PT_T4(a0)
    ld t5,PT_T5(a0)
    ld t6,PT_T6(a0)
    ld tp,PT_TP(a0)
    ld gp,PT_GP(a0)
    ld ra,PT_RA(a0)

    mv a0,zero		#对于子进程fork返回0
    sret
