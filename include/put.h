#ifndef PUT_H
#define PUT_H
#define UART16550A_DR (volatile unsigned char *)0x10000000

typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
//支持变长数组
void puti(int num);
int puts(const char *s);
void print(const char *fmt, ...);

#define putchar(c) (*(volatile unsigned char *)0x10000000)=(unsigned char)(c)

#endif