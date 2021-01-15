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
    if(fd>=NR_OPEN || !count)
        return -1; //wrong
    if(count==1)
        return 0;
    if(fd==1)
    {
        int cnt=0;
        while(cnt<count-1)
        {
            print("%c",buf[cnt++]);
        }
           return cnt;
    }

    
}
void *mmap (void *__addr, size_t __len, int __prot,
                   int __flags, int __fd, __off_t __offset)
{
    return do_mmap(current->mm, __addr, __len, __prot);
}

unsigned long get_unmapped_area(size_t length)
{
    unsigned long vm_suggested=0;
    struct vm_area_struct *pointer_to_vma=current->mm->mmap;
 
    for(;pointer_to_vma!=NULL;pointer_to_vma=pointer_to_vma->vm_next)
    {
        if(vm_suggested+length<=pointer_to_vma->vm_start)
        {
            break;
        }
        vm_suggested=pointer_to_vma->vm_end;
        
    }
    return vm_suggested;
    
}
void *do_mmap(struct mm_struct *mm, void *start, size_t length, int prot)
{
    unsigned long *addr_suggest;
    unsigned long start1;
    start1=(unsigned long)start;
    struct vm_area_struct *pointer_to_vma=mm->mmap;
    struct vm_area_struct *newvma;
    int flag=0;
    for(;pointer_to_vma;pointer_to_vma=pointer_to_vma->vm_next)
    {
        if((start1<=pointer_to_vma->vm_end && start1>=pointer_to_vma->vm_start) || (start1+length<=pointer_to_vma->vm_end && start1+length>=pointer_to_vma->vm_start))
            flag=1;
    }
    if(flag)
    {
        addr_suggest=(unsigned long*)get_unmapped_area(length);
        vma_insert(mm,addr_suggest,length,prot);
        return addr_suggest;
    }
    else 
    {
        vma_insert(mm,start,length,prot);
        return start;
    }
}
