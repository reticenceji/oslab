#include "test.h"
#include "put.h"

int os_test()
{
	const char *msg = "ZJU OS LAB 2             GROUP-G10\n";

    puts(msg);
	task_init();
	while(1) ;

    return 0;
}
