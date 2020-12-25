#include "sched.h"
#include "syscall.h"
long sys_getpid()
{
    return getpid();
}
long sys_write(unsigned int fd, const char* buf, size_t count)
{
    return 0;
}
