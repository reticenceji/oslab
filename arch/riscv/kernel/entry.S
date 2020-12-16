.section .text.entry
.globl exception_addr_s
.macro PUSH reg
    addi sp,sp,-8
    sd \reg,0(sp)
.endm				#宏
.macro POP reg
    ld \reg,0(sp)
    addi sp,sp,8
.endm
.equ SCAUSE_STI_NUM,0x8000000000000005
.equ SCAUSE_SPF_NUM,0xf
.equ SCAUSE_LPF_NUM,0xd
.equ SCAUSE_IPF_NUM,0xc

exception_addr_s:
	#保存所有的寄存器
	PUSH x2			 # 将原来的sp压栈
	# TODO 这里应该要切换到kernel sp的，之前好像一直没做？
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
store_page_fault_s:
	li t1,SCAUSE_SPF_NUM				#store page fault
	bne t0,t1,load_page_fault_s
	la a0,string_spf
	call puts
	call dead_loop
	j exception_retaddr_s
load_page_fault_s:
	li t1,SCAUSE_LPF_NUM				#load page fault
	bne t0,t1,instruction_page_fault_s	
	la a0,string_lpf
	call puts
	call dead_loop
	j exception_retaddr_s
instruction_page_fault_s:
	li t1,SCAUSE_IPF_NUM				#instruction page fault
	bne t0,t1,other_exception_s
	la a0,string_ipf
	call puts
	call dead_loop
	j exception_retaddr_s
other_exception_s:
	POP t0
	addi t0,t0,4
    csrw sepc, t0
	j exception_retaddr_s	

interupt_table_s:
	li t1,SCAUSE_STI_NUM			#对应于Supervisor timer interrupt
	bne t0,t1,other_interupt_s
	call sti_handler_s
    POP t0
    csrw sepc, t0
    ecall								#产生ecall_from_s_mode exception，重新设置mtimecmp
	j exception_retaddr_s
other_interupt_s:
	POP t0
    csrw sepc, t0
	j exception_retaddr_s	

exception_retaddr_s:							
	#恢复所有的寄存器
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
	sret
		
#第一个参数 a0 current
#第二个参数 a1 prev
.globl __switch_to
.type __switch_to,@function
__switch_to:
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
    
.section .rodata.entry
string_lpf: .string "load page fault\n"
string_spf: .string "store page fault\n"
string_ipf: .string "instruction page fault\n"
