.section .text.init
.globl _start
.macro PUSH reg
    sd  \reg, 0(sp)
	addi sp,sp,-8
.endm				#宏
.macro POP reg
	addi sp,sp,8
    ld  \reg, 0(sp)
.endm
.equ STACKSIZE, 0x100            #栈帧大小
.equ MTIME, 0x200bff8
.equ MTIMECMP, 0x2004000

_start:
    la t0, _bss_start
    la t1, _bss_end             #后面那个符号是lds给出的
loop_clear_bss:
    beq t0, t1, done_clear_bss
    sb zero, 0(t0) 
    addi t0, t0, 1
    j loop_clear_bss 
done_clear_bss:                 #循环的将.bss段内容设置为0
                                #必须clear bss 的原因
    li t0, MTIME
    ld t0, 0(t0)
    li t1, 1000000
    add t0, t0, t1
    li t1, MTIMECMP
    sd t0, 0(t1)                #设置mtimecmp = mtime+1000000
    
    li t0, 0x20
    csrw mideleg, t0            #设置委托
    li t0, 0xA0
    csrw mie, t0                #mie[mtie] = 1, sie[stie] = 1, 即允许时钟中断

    li t0, 0x7FF
    addi t0, t0, 0xB
    csrw mstatus, t0            #mstatus[mpp] = mstatus[mie] = mstatus[sie] = 1

    la t0, _start_retaddr
    csrw mepc, t0               #设置mret的返回地址
    la t0, exception_addr_m
# slli t0, t0, 2              #设置M mode Trap处理地址，为什么要左移两位？
    csrw mtvec, t0              #设置machine mode异常处理地址
    mret
    
_start_retaddr:
    la sp, stack_top
    la t0, exception_addr_s
# slli t0, t0, 2
    csrw stvec, t0              #设置supervisor mode异常处理地址
    call start_kernel
#   wfi                         #Wait For Interrupt, 暂停等待中断


#exception handlers
.align 2

exception_addr_m:
.globl exception_addr_m
#save context     
    addi sp, sp, -STACKSIZE
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


    csrr t0, mcause
	blt t0,zero,interupt_table_m		#如果最高位是0，那么是中断。
exception_table_m:
ecall_handler_m:            #I think the most optimum way is set mtvec[low]=1,use jump table
    li t1, MTIMECMP
    ld t1, 0(t1)
    li t0, 100000
    add t1, t1, t0
    li t0, MTIMECMP
    sd t1, 0(t0)                #mtimecmp += 100000
    li t0, 0x80
    csrs mie, t0                #mie[mtie] = 1
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0               #mepc += 4
    j exception_retaddr_m

interupt_table_m:
mti_handler_m:
    li t0, 0x80
    csrc mie, t0                #mie[mtie] = 0
    li t0, 0x20
    csrs mip, t0                #mip[stip] = 1
    j exception_retaddr_m

exception_retaddr_m:
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
    addi sp, sp, STACKSIZE
    mret

