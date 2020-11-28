#ifndef _VM_H
#define _VM_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#define KERNEL_START_P 0x80000000
#define KERNEL_END_P (0x80000000 + 16*0x100000)

#define KERNEL_START_V 0xffffffe000000000
#define KERNEL_END_V (0xffffffe000000000 + 16*0x100000)

#define PAGE_SIZE 4096
#define ENTRY_PER_PAGE (PAGE_SIZE / 8)
#define TEST_PAGE_NUM 20    //这里设计了比实际需要更多的数量

#define FRAME_SIZE 4096
#define ENTRY_PER_FRAME (FRAME_SIZE / 8)
#define TEST_FRAME_NUM 20

typedef unsigned long long uint64;

/* 循环队列实现物理内存管理 */
typedef unsigned long long frame_t[ENTRY_PER_FRAME];   //长度为4KB的连续内存空间
typedef unsigned long long page_t[ENTRY_PER_PAGE];
typedef struct frame_queue
{
    uint64 front;
    uint64 rear;
    uint64 capacity;
    frame_t *frame[TEST_FRAME_NUM+1];
} frame_queue_t;

/* 初始化frame_queue */
void init_frame_queue(frame_queue_t *fq);

/* 分配一个frame(dequeue) */
frame_t *alloc_frame(frame_queue_t *fq);

/* 回收一个frame(enqueue) */
void free_frame(frame_queue_t *fq);

/* 判断队列是否为满, 为满返回1, 否则返回0 */
static int fq_is_full(frame_queue_t *fq);

/* 判断队列是否为空, 为空返回1, 否则返回0 */
static int fq_is_empty(frame_queue_t *fq);


/* 页表映射的统一接口, 成功返回1, 失败返回0 */
int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);

/* 初始化地址映射 */
int paging_init();

#endif
