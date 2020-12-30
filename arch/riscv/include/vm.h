#ifndef _VM_H
#define _VM_H

#ifndef NULL
#define NULL 0
#endif

#include "sched.h"

#define REG_SIZE (8)
#define PAGE_SIZE 4096
#define ENTRY_PER_PAGE (PAGE_SIZE / 8)
#define TEST_PAGE_NUM 10000    //这里设计了比实际需要更多的数量

#define FRAME_SIZE 4096
#define ENTRY_PER_FRAME (FRAME_SIZE / 8)
#define TEST_FRAME_NUM 10000

#define UART_START_P 0x10000000
#define UART_START_V 0xffffffdf90000000
#define UART_SIZE 0x100

/* 我们把kernel space分成两部分
 * 第一部分是已经预先分配好的内存，包括代码、堆栈、一些slab
 * 第二部分是可供分配的内存，比如可供页表使用
 */
#define KERNEL_START_P 0x80000000   
#define KERNEL_SIZE 0x1000000       
#define KERNEL_END_P (KERNEL_START_P+KERNEL_SIZE)
#define KERNEL_ALLOCABLE_START_P KERNEL_END_P
#define KERNEL_ALLOCABLE_SIZE 0xF000000     
#define KERNEL_ALLOCABLE_END_P (KERNEL_ALLOCABLE_START_P+KERNEL_ALLOCABLE_SIZE)
/* 我们对Kernel Space采用偏移的映射方式，这就是偏移量
 * VP宏函数把kernel space的物理地址变成虚拟地址
 * PP宏函数把kernel space的虚拟地址变成物理地址
 */
#define MAP_OFFSET (KERNEL_START_V - KERNEL_START_P)
#define VP(X) ((uint64)(X)+MAP_OFFSET)
#define PP(X) ((uint64)(X)-MAP_OFFSET)      

#define KERNEL_START_V 0xffffffe000000000
#define KERNEL_END_V (KERNEL_START_V + KERNEL_SIZE)
#define KERNEL_ALLOCABLE_START_V KERNEL_END_V
#define KERNEL_ALLOCABLE_END_V (KERNEL_ALLOCABLE_START_V+KERNEL_ALLOCABLE_SIZE)
// 可以认为这一部分是task_struct的slab
#define KERNEL_TASK_START_P 0x80010000
#define KERNEL_TASK_START_V (KERNEL_TASK_START_P + KERNEL_START_V - KERNEL_START_P)
#define KERNEL_TASK_SIZE (1 * PAGE_SIZE)
// 可以认为这一部分是mm_struct的slab
#define MM_SIZE (1 * REG_SIZE)
#define MM_START_V (KERNEL_TASK_START_V + NR_TASKS * KERNEL_TASK_SIZE)

//lab5的用户进程的物理地址，本来应该是由kernel分配的
#define USER_TASK_START_P (0x84000000)
#define USER_TASK_START_V (0x0)
#define USER_TASK_SIZE (1 * PAGE_SIZE)
// TODO 用户栈？为什么需要这个呢？
#define USER_STACK_BOTTOM_V (0xffffffdf80000000)
#define USER_STACK_BOTTOM_P (USER_STACK_BOTTOM_V + USER_TASK_START_P - USER_TASK_START_V)
#define USER_STACK_SIZE     (1 * PAGE_SIZE)
#define USER_STACK_TOP_V    (USER_STACK_BOTTOM_V - USER_STACK_SIZE)
#define USER_STACK_TOP_P    (USER_STACK_TOP_V + USER_TASK_START_P - USER_TASK_START_V)

/* 页表的权限位，用或连接 */
#define FLAG_V 0x1
#define FLAG_R 0x2
#define FLAG_W 0x4
#define FLAG_X 0x8
#define FLAG_U 0x10
#define FLAG_G 0x11
#define FLAG_A 0x12
#define FLAG_D 0x14

/* 页表模式SV39 */
#define MODE_SV39 0x8000000000000000
#define KERNEL_SATP (MODE_SV39 | ((KERNEL_ALLOCABLE_START_P) >> 12))
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
void init_frame_queue();

/* 分配一个frame(dequeue) */
uint64 alloc_frame();

/* 回收一个frame(enqueue) */
void free_frame();

/* 页表映射的统一接口 */
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);

/* 初始化地址映射, 返回基地址*/
uint64* paging_init();

/* 映射内核页表 */
void kernel_mapping(uint64 *pgtbl);

int __is_page_open();
#endif
