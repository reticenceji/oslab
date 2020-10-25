#include "put.h"

void sti_handler()
{
	const static char prompt[]="\n[S] Supervisor Mode Timer Interrupt ";
	static int count=0;		//可以关注一下这两个被分配到哪个段了
	puts(prompt);
	puti(count);
	count++;
	return;
}