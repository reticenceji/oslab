#include "put.h"

void sti_handler()
{
	const static char prompt[]="\n[S] Supervisor Mode Timer Interrupt ";
	static int count=0,if_handred=0;		//可以关注一下这两个被分配到哪个段了
	if (if_handred==100)		//100个好快。。
	{
		puts(prompt);
		puti(count);
		count++;
		if_handred=0;
	}
	if_handred++;
	return;
}