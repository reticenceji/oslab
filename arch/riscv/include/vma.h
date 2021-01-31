#ifndef _VMA_H
#define _VMA_H

#include "stddef.h"
struct vm_area_struct *vma_find(uint64 va);
static struct vm_area_struct *vma_build(struct mm_struct *mm, void *start, size_t length, int prot);
int vma_insert(struct task_struct *tsk, void *start, size_t length, int prot);
int vma_delete(uint64 va);
int vma_split(uint64 va);
int vma_contiguous(uint64 from_v, uint64 to_v);
void vma_copy(struct mm_struct *mm);

#endif
