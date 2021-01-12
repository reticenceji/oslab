#include "types.h"
#include "vm.h"
#define PAGE_MASK (0xFFF)
extern struct task_struct *current;

static void change_protection(uint64 va_pte)
{
    *(uint64 *)va_pte = 
}

/* 根据所给的virtual address判断属于哪一个vma */
struct vm_area_struct *find_vma(uint64 va)
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

/* 检查从start到end的所有内容是不是已经被映射了 */
static int test_range(uint64 start,uint64 end,struct vm_area_struct *vma)
{

}

static void edit_vma(uint64 start,uint64 end)
{
    struct vm_area_struct *vma;
    vma = find_vma(start);
    if (vma->vm_start != start)
        
}

/* 从va_start到va_end的每一个页表项都执行func的操作 */
static inline int edit_page_table(uint64 va_start,uint64 va_end, void func(uint64 va_pte));


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
    if (end<start || __prot&(FLAG_R|FLAG_W|FLAG_X)) 
        return -1;          
    /* 中间存在没有映射的区域 */
    if (test_range(start,end,current->vma))
        return -1;

    edit_vma();
    edit_page_table(start,end,);
}