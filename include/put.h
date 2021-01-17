#ifndef _PUT_H
#define _PUT_H

#include "stddef.h"

void puti(int num);
int puts(const char *s);
void print(const char *fmt, ...);
void putX(uint64 num);
#define UART16550A_DR (volatile unsigned char *)(0xffffffdf90000000)
#define putchar(c) (* UART16550A_DR)=(unsigned char)(c)

#endif
