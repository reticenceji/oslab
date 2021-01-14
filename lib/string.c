#include "string.h"
/* memset函数按字节对内存块进行初始化,ch实际范围应该在0~~255 */
void* memset(void *dst, int c, uint n)
{
    for (uint i=0;i<n;i++)
        dst[i] = (char)c;
}

void* memmove(void *dst, const void *src, uint n)
{
    for (uint i=0;i<n;i++)
        dst[i] = src[i];
}