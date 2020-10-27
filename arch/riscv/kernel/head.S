.section .text.init
.globl _start
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
    li t0, 0x80
    csrw mie, t0                #mie[mtie] = 1, 即允许时钟中断
    csrw sie, t0                #sie[stie] = 1

    li t0, 0x7FF
    addi t0, t0, 0xB
    csrw mstatus, t0            #set mstatus's MPP to S Mode; mstatus[mie] = 1

    la t0, retaddr  
    csrw mepc, t0               #设置mret的返回地址
    la t0, exception_m_addr
    slli t0, t0, 2              #设置M mode Trap处理地址，为什么要左移两位？
    csrw mtvec, t0              #设置machine mode异常处理地址
    mret
    
retaddr:
    la sp, stack_top
    la t0, exception_s_addr
    slli t0, t0, 0x2
    csrw stvec, t0              #设置supervisor mode异常处理地址
    call start_kernel
    wfi                         #Wait For Interrupt, 暂停等待中断


#exception handlers
.align 3

exception_m_addr:
.globl trap_m
trap_m:
    addi sp, sp, -STACKSIZE

    sd t0, STACKSIZE-0x8(sp)
    sd t1, STACKSIZE-0x10(sp)
    csrr t0, mepc
    sd t0, STACKSIZE-0x18(sp)
    csrr t0, mstatus
    sd t0, STACKSIZE-0x20(sp)
    csrr t0, mie
    sd t0, STACKSIZE-0x28(sp)
    csrr t0, mip
    sd t0, STACKSIZE-0x30(sp)

#TIME INTERRUPT:
    lui t0, 0x80000
    slli t0, t0, 0x20
    addi t0, t0, 0x7            #t0 = 0x8000 0000 0000 0007
    csrr t1, mcause
    bne t1, t0, ecall_handler_m
    li t1, 0x20
    csrrs zero, mip, t1         #mip[stip]=1
    li t1, 0x80
    csrrc zero, mie, t1         #mie[mtie]=0
    j return_m

#ECALL:
ecall_handler_m:
    li t1, MTIMECMP
    ld t1, 0(t1)
    li t0, 100000
    add t1, t1, t0
    li t0, MTIMECMP
    sd t1, 0(t0)

return_m:
    ld t0, STACKSIZE-0x30(sp)
    csrw mip, t0
    ld t0, STACKSIZE-0x28(sp)
    csrw mie, t0
    ld t0, STACKSIZE-0x20(sp)
    csrw mstatus, t0
    ld t0, STACKSIZE-0x18(sp)
    csrw mepc, t0
    ld t1, STACKSIZE-0x10(sp)
    ld t0, STACKSIZE-0x8(sp)

    addi sp, sp, STACKSIZE
    mret

#exception_s_addr:
#.globl trap_s
#trap_s:
    #call trap_supervisor
    #sret
