#ifndef SYSCALL_H
#define SYSCALL_H

typedef unsigned long long size_t ;
typedef unsigned long long uintptr_t ;
//系统调用号
#define SYS_WRITE 64
#define SYS_GETPID 172
#define NR_OPEN 2

long sys_write(unsigned int fd, const char* buf, size_t count);
long sys_getpid();

#endif