#include "put.h"

void sti_handler_s()
{
	const static char prompt[]="\n[S] Supervisor Mode Timer Interrupt ";
	static int count=0,if_handred=0;		//可以关注一下这两个被分配到哪个段了
	do_timer();
	// if (if_handred==100)	
	// {
	// 	puts(prompt);
	// 	puti(count);
	// 	count++;
	// 	if_handred=0;
	// }
	// if_handred++;
	return;
}
