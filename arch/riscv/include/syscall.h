#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"
//系统调用号
#define SYS_WRITE 64
#define SYS_GETPID 172
#define NR_OPEN 2

long sys_write(unsigned int fd, const char* buf, size_t count);
long sys_getpid();

#endif