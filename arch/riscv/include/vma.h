#ifndef _VMA_H
#define _VMA_H
struct vm_area_struct *vma_find(uint64 va);
struct vm_area_struct *vma_build(struct mm_struct *mm, void *start, size_t length, int prot);
int vma_insert(struct mm_struct *mm, void *start, size_t length, int prot);
int vma_delete(uint64 va);
int vma_split(uint64 va);
int vma_contiguous(uint64 from_v, uint64 to_v);
struct vm_area_struct *vma_copy(struct mm_struct *mm);
#endif
