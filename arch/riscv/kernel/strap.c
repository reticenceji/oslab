#include "put.h"

void sti_handler_s()
{
	//const static char prompt[]="\n[S] Supervisor Mode Timer Interrupt ";
	static int count=0,if_hundred=0;		//可以关注一下这两个被分配到哪个段了
	//do_timer();
	if (if_hundred==100)	
	{
	// 	puts(prompt);
	// 	puti(count);
	// 	count++;
		if_hundred=0;
		do_timer();
	}
	if_hundred++;
	return;
}
