#include "put.h"
extern void do_timer();
void sti_handler_s()
{
	static int count=0,if_hundred=0;		//可以关注一下这两个被分配到哪个段了
	if (if_hundred==100)	
	{
		if_hundred=0;
		do_timer();
	}
	if_hundred++;
	return;
}
