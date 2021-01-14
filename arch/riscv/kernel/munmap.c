#include "sched.h"
#include "vma.h"
#include "types.h"
#include "vm.h"

void free_page_tables(uint64 pagetable, uint64 va, uint64 n, int free_frame)
{
    uint64 base = pagetable;
    uint64 tmp_va = va;
    int index;
    for (int i = 0; i < n; i++, tmp_va+=PAGE_SIZE)
    {
        index = (tmp_va>>30) & 0x1FF;
        base = VP(base[index]>>10);
        index = (tmp_va>>21) & 0x1FF;
        base = VP(base[index]>>10);
        index = (tmp_va>>12) & 0x1FF;
        base[index] = 0;
    }

    if (free_frame == 1)
    {
        for (int i = 0; i < n; i++, va+=PAGE_SIZE)
        {
            kfree(va);
        }
    }
    return ;
}

int munmap(void *start, size_t length)
{
    uint64 end = ((uint64)start + length);
    end = end - end%PAGE_SIZE + PAGE_SIZE-1;
    start -= start % PAGE_SIZE;
    n = (end - start + 1) / PAGE_SIZE;

    if (vma_contiguous(start, end) == -1)
    {
        return -1;
    }

    uint64 pgtbl = VP(current->mm->satp & ~PAGE_MASK);
    free_page_tables(pgtbl, (uint64)start, n, 1);
    
    struct vm_area_struct *ptr_start = vma_find((uint64)start);
    struct vm_area_struct *ptr_end = vma_find(end);
    if (ptr_start == ptr_end)
    {
        vma_split(start);   //[vm_start, vm_end) --> [vm_start, start) [start, vm_end)
        vma_split(end);     //[start, vm_end) --> [start, end) [end, vm_end)
        //vma_delete(vma_find(start)->vm_start);
        vma_delete(ptr_start->next->vm_start);
        return 0;
    }
    while (ptr_start != ptr_end)
    {
        ptr_start = ptr_start->vm_next;
        vma_delete(ptr_start->prev->vm_start);
    }
    vma_split(end);
    vma_delete(ptr_end->vm_start);
    return 0;
}
