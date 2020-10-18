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

    addi t0,zero, 0x80 
    csrw mideleg, t0    #设置委托
    csrw mie, t0        #Xie[7]=1, 即允许时钟中断   
    
    addi t0, zero, 0x7FF
    addi t0, t0, 0xB
    csrw mstatus, t0    #set mstatus's MPP to S mode, 允许产生中断

    la t0, retaddr  
    csrw mepc, t0       #设置mret的返回地址
    la t0, exception_m_addr
    slli t0, t0, 2      #设置M mode Trap处理地址，为什么要左移两位？
    csrw mtvec, t0      #设置machine mode异常处理地址
    mret
    
retaddr:
    la sp, stack_top
    la t0, exception_s_addr
    slli t0, t0, 2
    csrw stvec, t0      #设置supervisor mode异常处理地址
    call start_kernel
    wfi                 #Wait For Interrupt, 暂停等待中断

.align 3
exception_m_addr:
exception_s_addr:
