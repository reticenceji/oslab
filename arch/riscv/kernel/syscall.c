#include "put.h"
#include "syscall.h"
#define NR_OPEN 2//mostly open one case


long sys_getpid()
{
    return getpid();
}
long sys_write(unsigned int fd, const char* buf, size_t count)
{
//sys_write(unsigned int fd, const char* buf, size_t count)
    if(fd>=NR_OPEN||count<0)
    return -1; //wrong
    if(!count)
    return 0;
    if(fd==1)
    {
        print("%s",buf);
        return count-1;//dismiss '\n'
    }

    
}
