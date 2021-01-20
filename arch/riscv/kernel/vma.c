#include "sched.h"
#include "vma.h"
#include "slub.h"
#include "../../../include/stddef.h"
struct vm_area_struct *vma_find(uint64 va)
{
    struct vm_area_struct *ptr = current->mm->mmap;
    while (ptr!=NULL)
    {
        if (ptr->vm_start <= va && va<ptr->vm_end)
            return ptr;
        ptr = ptr->vm_next;
    }
    return ptr;
}

struct vm_area_struct *vma_build(struct mm_struct *mm, void *start, size_t length, int prot)
{
    struct vm_area_struct *new_node = (struct vm_area_struct *)kmalloc(VMA_SIZE);
    new_node->vm_start = (uint64)start;
    new_node->vm_end = (uint64)start + length;
    new_node->vm_mm = mm;
    //实验指导里好像没提vm_page_prot的赋值
    //new_node->vm_page_prot = (pgprot_t)prot;
    new_node->vm_flags = (uint64)prot;
    new_node->vm_prev = new_node->vm_next = NULL;
    return new_node;
}

void vma_copy(struct mm_struct *new_mm)
{
    struct vm_area_struct *copied_mmap = current->mm->mmap;
    struct vm_area_struct *ptr_move = new_mm->mmap;
    while (copied_mmap)
    {
        if (new_mm->mmap == NULL)
        {
            new_mm->mmap = vma_build(new_mm, copied_mmap->vm_start, copied_mmap->vm_end, copied_mmap->vm_flags);
            ptr_move = new_mm->mmap;
        }
        else
        {
            ptr_move->vm_next = vma_build(new_mm, copied_mmap->vm_start, copied_mmap->vm_end, copied_mmap->vm_flags);
            ptr_move->vm_next->vm_prev = ptr_move;
            ptr_move = ptr_move->vm_next;
        }
        copied_mmap = copied_mmap->vm_next;
    }
}


int vma_insert(struct mm_struct *mm, void *start, size_t length, int prot)
{
    struct vm_area_struct *ptr = current->mm->mmap;
    struct vm_area_struct *node = vma_build(mm, start, length, prot);

    if (node->vm_end <= ptr->vm_start)
    {
        node->vm_next = ptr;
        ptr->vm_prev = node;
        current->mm->mmap = node;
        return 0;
    }
    while (ptr->vm_next != NULL)
    {
        if (ptr->vm_next->vm_start >= node->vm_end
            && ptr->vm_end <= node->vm_start)
        {
            ptr->vm_next->vm_prev = node;
            node->vm_next = ptr->vm_next;
            node->vm_prev = ptr;
            ptr->vm_next = node;
            return 0;
        }
        ptr = ptr->vm_next;
    }
    if (ptr->vm_end <= node->vm_start)
    {
        node->vm_prev = ptr;
        node->vm_next = NULL;
        ptr->vm_next = node;
        return 0;
    }
    return -1;
}

/* 只要va在某个vma的范围内, 就delete这个vma */
int vma_delete(uint64 va)
{
    struct vm_area_struct *ptr = current->mm->mmap;
    if (ptr->vm_start <= va && va < ptr->vm_end)
    {
        current->mm->mmap = ptr->vm_next;
        kfree(ptr);
        return 0;
    }
    while (ptr->vm_next != NULL)
    {
        if (ptr->vm_next->vm_start <= va && va < ptr->vm_next->vm_end)
        {
            struct vm_area_struct *tmp = ptr->vm_next;
            if (ptr->vm_next->vm_next != NULL)
            {
                ptr->vm_next->vm_next->vm_prev = ptr;
                ptr->vm_next = ptr->vm_next->vm_next;
                kfree(tmp);
                return 0;
            }
            else
            {
                ptr->vm_next = NULL;
                kfree(tmp);
                return 0;
            }
        }
        ptr = ptr->vm_next;
    }
    if (ptr->vm_start <= va && va < ptr->vm_end)
    {
        ptr->vm_prev->vm_next = NULL;
        kfree(ptr);
        return 0;
    }
    return -1;
}

/* 拆分结果为[vm_start, va) [va, vm_end) */
int vma_split(uint64 va)
{
    struct vm_area_struct *ptr = current->mm->mmap;
    if (ptr->vm_start < va && va < ptr->vm_end)
    {
        struct vm_area_struct *new_node = vma_build(ptr->vm_mm, ptr->vm_start,
                                                    va-ptr->vm_start, ptr->vm_flags);
        new_node->vm_next = ptr;
        current->mm->mmap = new_node;
        ptr->vm_start = va;
        ptr->vm_prev = current->mm->mmap;
        return 0;
    }

    ptr = vma_find(va);
    if (ptr == NULL)
    {
        return -1;
    }
    else if (va == ptr->vm_start)
    {
        return 0;
    }
    else
    {
        struct vm_area_struct *new_node = vma_build(ptr->vm_mm, ptr->vm_start,
                                                    va-ptr->vm_start, ptr->vm_flags);
        new_node->vm_prev = ptr->vm_prev;
        new_node->vm_next = ptr;
        ptr->vm_prev->vm_next = new_node;
        ptr->vm_prev = new_node;
        return 0;
    }
    return -1;
}

int vma_contiguous(uint64 from_v, uint64 to_v)
{
    struct vm_area_struct *ptr_from = vma_find(from_v);
    struct vm_area_struct *ptr_to = vma_find(to_v);
    if (ptr_from == NULL || ptr_to == NULL)
    {
        return -1;
    }
    if (ptr_from == ptr_to)
    {
        return 0;
    }
    while (ptr_from != ptr_to)
    {
        if (ptr_from->vm_next->vm_start != ptr_from->vm_end)
        {
            return -1;
        }
        ptr_from = ptr_from->vm_next;
    }
    return 0;
}
