#ifndef _PUT_H
#define _PUT_H

#include "stddef.h"

void puti(int num);
int puts(const char *s);
void print(const char *fmt, ...);
void putX(unsigned long long num);
#define UART16550A_DR (volatile unsigned char *)0x10000000
#define putchar(c) (*(volatile unsigned char *)0x10000000)=(unsigned char)(c)

#endif
