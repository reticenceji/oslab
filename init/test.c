#include "test.h"
#include "put.h"

int os_test()
{
	const char *msg = "ZJU OS LAB 3             GROUP-G10\n";

    puts(msg);
//	print("hello %s %d %c","world",10,'d');
	task_init();
	//while(1) ;

    return 0;
}
