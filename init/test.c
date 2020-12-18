#include "test.h"
#include "put.h"
extern void task_init();
int os_test()
{
	const char *msg = "ZJU OS LAB 4             GROUP-G10\n";

    puts(msg);
	task_init();

    return 0;
}
