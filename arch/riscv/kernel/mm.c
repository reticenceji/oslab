#include "types.h"
#include "sched.h"
#include "vm.h"
#include "vma.h"
#define PAGE_MASK (0x3FF)
#define FLAG2PAGE_PROT(FLAG) (((FLAG)&0x7)>>1)

/* 检查从start到end的所有内容是不是已经被映射了 */
static int test_range(uint64 start,uint64 end,struct vm_area_struct *vma)
{
    struct vm_area_struct *vma,*ins_vma;
    uint64 tmp_end;
    for (;start<end;start = vma->vm_end)
    {
        vma = vma_find(start);
        if (vma==NULL) return 0;
    }
    return 1;
}

/* 修改从start到end的映射，前提是虚拟地址已经连续映射*/
static inline void edit_vma(uint64 start,uint64 end,int protection)
{
    struct vm_area_struct *vma;
    uint64 tmp_end;
    vma = vma_find(start);
    if (vma->vm_start != start) 
        vma_insert(current->mm, vma->vm_start, start - vma->vm_start, protection);
    while (1)
    {
        tmp_end = vma->vm_end;
        if (tmp_end>=end) break;
        vma_delete(tmp_end-1);
        vma=vma_find(tmp_end);
    }
    vma_insert(current->mm, start, end-start, protection);

    if (vma->vm_end != tmp_end)
        vma_insert(current->mm, end, vma->vm_end-end,protection);
}

/* 从va_start到va_end的每一个页表项都执行func的操作 */
static inline void edit_page_table(uint64 va_start,uint64 va_end, int protection)
{
    uint64* base =  VP(current->mm->satp & ~PAGE_MASK); //一级页表base
    uint64* va;
    int index;
    for (va = va_start;va<va_end;va+=PAGE_SIZE)
    {
        index = (va>>30) & 0x1FF;
        base = VP(base[index]>>10);    //二级页表base
        index = (va>>21) & 0x1FF;
        base = VP(base[index]>>10);    //三级页表base
        index = (va>>12) & 0x1FF;               
        base[index] = base[index] & 0xE | protection ;  //重新修改权限位
    }
}

/* 参考Linux1.2.13 的实现
 * 检查参数的合法性
 * 首先要是已经被映射在用户空间的，检查VMA
 * 修改VMA
 * 修改Page table，需要手动walk page table
 */
int mprotect (void *__addr, size_t __len, int __prot)
{
    uint64 start,end;
    uint64 vma,pte;
    /* 对齐 */
    if (!((uint64)__addr & ~PAGE_MASK))
        return -1;
    len = (len+PAGE_MASK) & ~PAGE_MASK; 
    start = __addr;
    end = __addr+end;
    /* overflow, 或者试图修改别的位 */
    if (end<start || __prot&(0xF)) 
        return -1;          
    /* 中间存在没有映射的区域 */
    if (test_range(start,end,current->vma))
        return -1;

    edit_vma(start,end,__prot);
    edit_page_table(start,end,FLAG2PAGE_PROT(__prot));
    return 0;
}
//===================
static void free_page_tables(uint64 pagetable, uint64 va, uint64 n, int free_frame)
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

