#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"
//系统调用号
#define SYS_WRITE 64
#define SYS_GETPID 172
#define SYS_MUNMAP 215
#define SYS_FORK 220
#define SYS_MMAP 222
#define SYS_MPROTECT 226

#define NR_OPEN 2

long sys_write(unsigned int fd, const char* buf, size_t count);
long sys_getpid();
void *mmap (void *__addr, size_t __len, int __prot,
                   int __flags, int __fd, __off_t __offset);
void *do_mmap(struct mm_struct *mm, void *start, size_t length, int prot);
#endif
