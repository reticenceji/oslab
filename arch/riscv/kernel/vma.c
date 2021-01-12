#include "sched.h"
#include "types.h"
extern struct task_struct *current;

struct vm_area_struct *vma_find(uint64 va)
{
    struct vm_area_struct *ptr = current->mm->mmap;
    while (ptr!=NULL)
    {
        if (ptr->vm_start <= va && va<ptr->vm_end)
            return ptr;
        ptr = ptr->next;
    }
    return ptr;
}