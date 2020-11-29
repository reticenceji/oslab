#ifndef _VM_H
#define _VM_H

#ifndef NULL
#define NULL 0
#endif

#define UART_START 0x10000000
#define UART_SIZE 1

#define KERNEL_SIZE 0x1000000
#define KERNEL_START_P 0x80000000
#define KERNEL_END_P (KERNEL_START_P+KERNEL_SIZE)

#define KERNEL_START_V 0xffffffe000000000
#define KERNEL_END_V (KERNEL_START_V + KERNEL_SIZE)

#define PAGE_SIZE 4096
#define ENTRY_PER_PAGE (PAGE_SIZE / 8)
#define TEST_PAGE_NUM 20    //这里设计了比实际需要更多的数量

#define FRAME_SIZE 4096
#define ENTRY_PER_FRAME (FRAME_SIZE / 8)
#define TEST_FRAME_NUM 20

/* 页表的权限位，用或连接 */
#define FLAG_V 0x1
#define FLAG_R 0x2
#define FLAG_W 0x4
#define FLAG_X 0x8
#define FLAG_U 0x10
#define FLAG_G 0x11
#define FLAG_A 0x12
#define FLAG_D 0x14

typedef unsigned long long uint64;  //指向一个地址

/* 循环队列实现物理内存管理 */
typedef struct frame_queue
{
    uint64 front;
    uint64 rear;
    uint64 capacity;
    uint64 frame[TEST_FRAME_NUM+1];
} frame_queue_t;

/* 初始化frame_queue */
void init_frame_queue(frame_queue_t *fq);

/* 分配一个frame(dequeue) */
uint64 alloc_frame(frame_queue_t *fq);

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
