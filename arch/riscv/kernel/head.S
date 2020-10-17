.section .text.init
.globl _start
EXTERN(_bss_start)
EXTERN(_bss_end)         #其实我也不确定这样对不对

_start:
    la t0, _bss_start
    la t1, _bss_end      #后面那个符号是lds给出的
_loop_clear_bss:
    beq t0,t1 _done_clear_bss
    sb zero,0(t0) 
    add t0,t0,1
    j _loop_clear_bss 
_done_clear_bss:        #循环的将.bss段内容设置为0
                        #必须clear bss 的原因 
    
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
