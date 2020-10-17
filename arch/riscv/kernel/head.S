.section .text.init
.globl _start
_start:
    li a0, 0
    addi a0, a0, 0x7FF
    addi a0, a0, 1
    csrw mstatus, a0    #set mstatus's MPP to S mode

    la a0, retaddr  
    csrw mepc, a0       #设置mret的返回地址
    la a0, exception_m_addr
    slli a0, a0, 2
    csrw mtvec, a0       #设置machine mode异常处理地址
    mret
    
retaddr:
    la sp, stack_top
    la a0, exception_s_addr
    slli a0, a0, 2
    csrw stvec, a0      #设置supervisor mode异常处理地址
    call start_kernel
    wfi                 #Wait For Interrupt, 暂停等待中断

exception_m_addr:
exception_s_addr:
