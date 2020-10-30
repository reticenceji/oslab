.section .text.entry
.globl exception_addr_s
.macro PUSH reg
    sd  \reg, 0(sp)
	addi sp,sp,-8
.endm				#宏
.macro POP reg
	addi sp,sp,8
    ld  \reg, 0(sp)
.endm
.equ SCAUSE_STI_MASK,0x5

exception_addr_s:
	PUSH x2			# 将原来的sp压栈
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
	PUSH x15
	PUSH x16
	PUSH x17
	PUSH x18
	PUSH x19
	PUSH x20
	PUSH x21
	PUSH x22
	PUSH x23
	PUSH x24
	PUSH x25
	PUSH x26
	PUSH x27
	PUSH x28
	PUSH x29
	PUSH x30
	PUSH x31
    csrr t0, sstatus
	PUSH t0
    csrr t0, scause
    PUSH t0
    csrr t0, stval
    PUSH t0
    csrr t0, sepc
    PUSH t0
	
    csrr t0,scause
	blt t0,zero,interupt_table_s		#如果最高位是0，那么是中断。
exception_table_s:					

interupt_table_s:
	andi t1,t0,SCAUSE_STI_MASK		#对应于Supervisor timer interrupt
	beq t1,zero,exception_retaddr_s
	call sti_handler_s
    POP t0
    csrw sepc, t0
	j exception_retaddr_s	

exception_retaddr_s:
#csrw sip,zero				#表示中断处理好了
    
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
	ecall			#产生ecall_from_s_mode exception
	sret			

