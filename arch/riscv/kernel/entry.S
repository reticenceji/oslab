.globl exception_addr_s
.macro PUSH reg
    addi sp,sp,-8
    sd \reg,0(sp)
.endm				
.macro POP reg
    ld \reg,0(sp)
    addi sp,sp,8
.endm
.equ SCAUSE_STI_NUM,0x8000000000000005
.equ CODE_MASK,0xFF 		#取中断/异常号的mask

.section .text.entry
exception_addr_s:
	# TODO 我打算重构这里，这样PUSH效率好像有点低，不过问题倒也不大
	csrrw sp,sscratch,sp	#sscratch存的是kernel stack的地址，和Linux的实现方式有一定区别
	PUSH x2			 		
	PUSH x1			
    PUSH x3
	PUSH x4
	PUSH x5
	PUSH x6
	PUSH x7
	PUSH x8
	PUSH x9
	PUSH x10			
	PUSH x11			
	PUSH x12
	PUSH x13
	PUSH x14
	PUSH x15			# x15 == sp+0x100
	PUSH x16
	PUSH x17			# a7 == sp+0x90
	PUSH x18
	PUSH x19
	PUSH x20
	PUSH x21			# x21 == sp+0x70
	PUSH x22
	PUSH x23
	PUSH x24
	PUSH x25
	PUSH x26
	PUSH x27
	PUSH x28
	PUSH x29
	PUSH x30
	PUSH x31			# x31 == sp + 0x20
    csrr t0, sstatus	
	PUSH t0				# sstatus == sp+0x18
    csrr t0, scause
    PUSH t0				# scause == sp+0x10
    csrr t0, stval
    PUSH t0				# stval == sp+0x8
    csrr t0, sepc
    PUSH t0				# sepc == sp+0x0
	
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
	POP t0
    csrw sepc, t0
	POP t0
	csrw stval, t0 
	POP t0
	csrw scause,t0
    POP t0
    csrw sstatus, t0
	POP x31
	POP x30
	POP x29
	POP x28
	POP x27
	POP x26
	POP x25
	POP x24
	POP x23
	POP x22
	POP x21
	POP x20
	POP x19
	POP x18
	POP x17
	POP x16
	POP x15
	POP x14
	POP x13
	POP x12
	POP x11
	POP x10
	POP x9
	POP x8
	POP x7
	POP x6
	POP x5
	POP x4
	POP x3
	POP x1
	POP x2
	csrrw sp,sscratch,sp
	sret
#第一个参数 a0 current.mm.satp
.globl __switch_page
.type __switch_page,@function

__switch_page:
	csrw satp, a0
	sfence.vma zero, zero
	ret	
#第一个参数 a0 current
#第二个参数 a1 prev
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

.globl __is_page_open
.type __is_page_open,@function
__is_page_open:
	csrr a0,satp
	srli a0,a0,60 
	ret

#第一个参数 a0 stack
.globl ret_from_fork
.type ret_from_fork,@function
ret_from_fork:
	ld t0,0x0(a0)
	csrw sepc,t0
	ld t0,0x8(a0)
	csrw stval,t0
	ld t0,0x10(a0)
	csrw scause,t0
	ld t0,0x18(a0)
	csrw sstatus,t0
	ld x31,0x20(a0)
	ld x30,0x28(a0)
	ld x29,0x30(a0)
	ld x28,0x38(a0)
	ld x27,0x40(a0)
	ld x26,0x48(a0)
	ld x25,0x50(a0)
	ld x24,0x58(a0)
	ld x23,0x60(a0)
	ld x22,0x68(a0)
	ld x21,0x70(a0)
	ld x20,0x78(a0)
	ld x19,0x80(a0)
	ld x18,0x88(a0)
	ld x17,0x90(a0)
	ld x16,0x98(a0)
	ld x15,0xa0(a0)
	ld x14,0xa8(a0)
	ld x13,0xb0(a0)
	ld x12,0xb8(a0)
	ld x11,0xc0(a0)
	ld x10,0xc8(a0)
	ld x9,0xd0(a0)
	ld x8,0xd8(a0)
	ld x7,0xe0(a0)
	ld x6,0xe8(a0)
	ld x5,0xf0(a0)
	ld x4,0xf8(a0)
	ld x3,0x100(a0)
	ld x2,0x108(a0)
	ld x1,0x110(a0)

	csrrw sp,sscratch,sp
	mv a0,zero		#对于子进程fork返回0
	sret

/* 如果debug成功了，试试把原来的push改成 
	.equ PT_REG_LEN,-0x110
	csrrw sp,sscratch,sp
	sd sp,0x10(sp)
	add sp,sp,-0x118
	sd x2,0x110(sp)
	sd x3,0x100(sp)
	sd x4,0xf8(sp)
	sd x5,0xf0(sp)
	sd x6,0xe8(sp)
	sd x7,0xe0(sp)
	sd x8,0xd8(sp)
	sd x9,0xd0(sp)
	sd x10,0xc8(sp)
	sd x11,0xc0(sp)
	sd x12,0xb8(sp)
	sd x13,0xb0(sp)
	sd x14,0xa8(sp)
	sd x15,0xa0(sp)
	sd x16,0x98(sp)
	sd x17,0x90(sp)
	sd x18,0x88(sp)
	sd x19,0x80(sp)
	sd x20,0x78(sp)
	sd x21,0x70(sp)
	sd x22,0x68(sp)
	sd x23,0x60(sp)
	sd x24,0x58(sp)
	sd x25,0x50(sp)
	sd x26,0x48(sp)
	sd x27,0x40(sp)
	sd x28,0x38(sp)
	sd x29,0x30(sp)
	sd x30,0x28(sp)
	sd x31,0x20(sp)
	csrr t0,sstatus
	sd t0,0x18(sp)
	csrr t0,scause
	sd t0,0x10(s0)
	csrr t0,stval
	sd t0,0x8(sp)
	csrr t0,sscratch
	sd t0,0(sp)



pop改成

	ld t0,0(sp)
	csrw sepc,t0
	ld t0,8(sp)
	csrw stval,t0
	ld t0,0x10(sp)
	csrw scause,t0
	ld t0,0x18(sp)
	csrw sstatus,t0
	ld x31,0x20(sp)
	ld x30,0x28(sp)
	ld x29,0x30(sp)
	ld x28,0x38(sp)
	ld x27,0x40(sp)
	ld x26,0x48(sp)
	ld x25,0x50(sp)
	ld x24,0x58(sp)
	ld x23,0x60(sp)
	ld x22,0x68(sp)
	ld x21,0x70(sp)
	ld x20,0x78(sp)
	ld x19,0x80(sp)
	ld x18,0x88(sp)
	ld x17,0x90(sp)
	ld x16,0x98(sp)
	ld x15,0xa0(sp)
	ld x14,0xa8(sp)
	ld x13,0xb0(sp)
	ld x12,0xb8(sp)
	ld x11,0xc0(sp)
	ld x10,0xc8(sp)
	ld x9,0xd0(sp)
	ld x8,0xd8(sp)
	ld x7,0xe0(sp)
	ld x6,0xe8(sp)
	ld x5,0xf0(sp)
	ld x4,0xf8(sp)
	ld x3,0x100(sp)
	ld x1,0x110(sp)
	add sp,sp,0x118
	ld x2,-0x10(sp)
	csrrw sp,sscratch,sp
	sret
*/
