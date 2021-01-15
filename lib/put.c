#include"put.h"

int puts(const char *s)
{
    while (*s != '\0')
    {
        *UART16550A_DR = (unsigned char)(*s);
        s++;
    }
    return 0;
}
static char itoch(int x)
{
    if (x >= 0 && x <= 9)
    {
        return (char)(x + 48);
    }
    return 0;
}
void puti(int x)
{
    int digit = 1, tmp = x;
    while (tmp >= 10)
    {
        digit *= 10;
        tmp /= 10;
    }
    while (digit >= 1)
    {
        *UART16550A_DR = (unsigned char)itoch(x/digit);
        x %= digit;
        digit /= 10;
    }
    return;
}

void putX(uint64 x)
{
    uint64 digit = 1, tmp = x;
    while (tmp >= 16)
    {
        digit = digit << 4;
        tmp = tmp >> 4;
    }
    while (digit != 0)
    {
        *UART16550A_DR = (unsigned char) (x/digit<10?x/digit+'0':x/digit-10+'a');
        x %= digit;
        digit = digit >> 4;
    }
    return;
}

/* 变长数组的用法 https://www.tutorialspoint.com/c_standard_library/stdarg_h.htm
 * 只实现了简单的%d %c %s
 */
void print(const char *fmt, ...)
{
    va_list ap;
    int d;
    uint64 x;
    char c, *s;

    va_start(ap, fmt);
    while (*fmt)
    {
        if (*fmt=='%')
        {
        switch(*++fmt)
        {
            case 's': /* 字符串 */
                    s = va_arg(ap, char *);
                    puts(s);
                    break;
            case 'd': /* 数字int */
                    d = va_arg(ap, int);
                    puti(d);
                    break;
            case 'l': /* 数字long */
                    d = va_arg(ap, long);
                    puti(d);
                    break;
            case 'X': /* 十六进制输出 uint64 */
                    x = va_arg(ap, uint64 );
                    putX(x);
                    break;
            case 'c': /* 字符 */
                    c = (char)va_arg(ap, int);
                    putchar(c);
            break;
        }
        }
        else
            putchar(*fmt);
        fmt++;
    }
    va_end(ap);
}
