.section .text
.globl exception_s_addr
exception_s_addr:
	sd a0,-8(sp)	
	sd t0,-16(sp)	
	sd t1,-24(sp)
	add sp,sp,-24	#题目说要保存所有的寄存器？

STI_handle:			#直接来到时钟异常处理函数，因为这次就这一个异常
	la a0, prompt	#加载第一个参数，看汇编代码是寄存器传参数
	call puts		#puts在全局符号表出现，并且是global，我猜可以直接用

	ld a0, count	#加载数字
	call puti     
	add a0,a0,1
	sd a0, count, zero  

	csrr t0,sip
	li t1, -0x21
	and t0,t0,t1	
    csrw sip,t0		#mip[stip]=0，处理完毕

return_s:
	sd t1,0(sp)
	sd t0,8(sp)
	sd a0,16(sp)	
	add sp,sp,24	
	ecall			#产生ecall_from_s_mode exception
	sret			

.section .data
prompt: 
	.string "\n[S] Supervisor Mode Timer Interrupt "
count:
	.8byte 0		#这个属于static变量，上面那个属于const，也许不应该出现在这里。
