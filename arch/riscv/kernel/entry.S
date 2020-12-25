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

.section .rodata.entry
string_lpf: .string "load page fault\n"
string_spf: .string "store page fault\n"
string_ipf: .string "instruction page fault\n"

.section .text.entry
exception_addr_s:
	# TODO 我打算重构这里，这样PUSH效率好像有点低，不过问题倒也不大
	csrrw sp,sscratch,sp	#sscratch存的是kernel stack的地址，和Linux的实现方式有一定区别
	PUSH x2			 		# 将原来的sp压栈
	PUSH x1			
    PUSH x3
	PUSH x4
	PUSH x5
	PUSH x6
	PUSH x7
	PUSH x8
	PUSH x9
	PUSH x10			# a0 == sp+0x128
	PUSH x11			# a1 == sp+0x120
	PUSH x12
	PUSH x13
	PUSH x14
	PUSH x15			# x15 == sp+0x100
	PUSH x16
	PUSH x17
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
# 	la t1,exception_table_s
# 	andi t0,t0,CODE_MASK				#t0是异常编号
# 	slli t0,t0,2
# 	add t1,t1,t0
# 	jr t1
# exception_table_s:
# 	j other_exception_s				#code==0
# 	j other_exception_s	
# 	j other_exception_s	
# 	j other_exception_s	
# 	j other_exception_s	
# 	j other_exception_s				#code==5
# 	j other_exception_s	
# 	j other_exception_s	
# 	j ecall_from_user_s				#code==8
# 	j other_exception_s	
# 	j other_exception_s				#code==A
# 	j other_exception_s	
# 	j instruction_page_fault_s		#code==C
# 	j load_page_fault_s				#code==D
# 	j other_exception_s	
# 	j store_page_fault_s			#code==F

# store_page_fault_s:
# 	la a0,string_spf
# 	call puts
# 	POP t0
# 	addi t0,t0,4
#     csrw sepc, t0
# 	j exception_retaddr_s
# load_page_fault_s:
# 	la a0,string_lpf
# 	call puts
# 	POP t0
# 	addi t0,t0,4
#     csrw sepc, t0
# 	j exception_retaddr_s
# instruction_page_fault_s:
# 	la a0,string_ipf
# 	call puts
# 	wfi
# 	j exception_retaddr_s
# ecall_from_user_s:
# 	csrr a0,scause		#第一个参数 scause
# 	csrr a1,sepc		#第二个参数 sepc
# 	mv a2,sp			#第三个参数 regs
# 	call handler_s
# 	POP t0
# 	addi t0,t0,4
#     csrw sepc, t0
# 	j exception_retaddr_s
# other_exception_s:
# 	POP t0
# 	addi t0,t0,4
#     csrw sepc, t0
# 	j exception_retaddr_s	

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
	csrrw sp,sscratch,sp
	sret
		
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

	ld t0,-16(a1)
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
