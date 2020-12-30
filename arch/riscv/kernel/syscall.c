#include "put.h"
#include "syscall.h"
#include "sched.h"
#define NR_OPEN 2//0 标准输入 1标准输出 2标准错误，这次只用1


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
        puts(buf);
        return count-1;//dismiss '\0'
    }

    
}
