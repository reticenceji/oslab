#include "buddy.h"
#include "vm.h"

#define LEFT_LEAF(INDEX) (((INDEX)<<1)+1)
#define RIGHT_LEAF(INDEX) (((INDEX)<<1)+2)
#define PARENT(INDEX) (((INDEX)-1)>>1)
#define MAX(A,B) ((A)>(B)?(A):(B))
#define OFFSET2VA(OFFSET) (((OFFSET)<<12)+KERNEL_START_V)
#define OFFSET2PA(OFFSET) (((OFFSET)<<12)+KERNEL_START_P)
#define VA2OFFSET(VA) ((VA-KERNEL_START_V)>>12)

extern uint64 _start ;
extern uint64 _end;
static struct buddy buddy_system;
static int bitmap[(KERNEL_SIZE + KERNEL_ALLOCABLE_SIZE)/PAGE_SIZE*2];
static inline fixsize(int size);
static void init_bitmap(int index,int size);

/* 把size对齐到2^n */
static inline int fixsize(int size)
{
    int fix_size=1;
    size--;
    while (size)
    {
        fix_size<<=1;
        size>>=1;
    }
    return fix_size;
}

static void init_bitmap(int index,int size)
{
    buddy_system.bitmap[index] = size;
    if (size>1)
    {
        init_bitmap(index*2+1,size/2);
        init_bitmap(index*2+2,size/2);
    }
}

void init_buddy_system(void)
{   
    buddy_system.size = (KERNEL_SIZE + KERNEL_ALLOCABLE_SIZE)/ PAGE_SIZE; 
    buddy_system.bitmap = bitmap;
    init_bitmap(0,buddy_system.size);
    //把kernel的代码标记为已经分配
    alloc_pages(((uint64)&_end-(uint64)&_start)/PAGE_SIZE);
}

void *alloc_pages(int size) {
    unsigned index = 0;
    unsigned node_size;
    uint64 va;

    size = fixsize(size);
    buddy_system.bitmap = bitmap;
    /* 如果小于的话说明空间不够了 */
    if (buddy_system.bitmap[index] < size)
        return NULL;

    /* 在bitmap里寻找合适的index */
    for(node_size = buddy_system.size; node_size != size; node_size /= 2) {
        if (buddy_system.bitmap[LEFT_LEAF(index)] >= size)
            index = LEFT_LEAF(index);
        else
            index = RIGHT_LEAF(index);
    }
    /* 算出在大页表里的偏移 */
    if (__is_page_open())
    {
        va = OFFSET2VA( (index + 1) * node_size - buddy_system.size);
        print("[S] Buddy allocate addr: 0x%X\n",va);
    }
    else
        va = OFFSET2PA( (index + 1) * node_size - buddy_system.size);
    
    buddy_system.bitmap[index] = 0;
    //返回修改父节点的值
    while (index>0) {
        index = PARENT(index);
        buddy_system.bitmap[index] = MAX(buddy_system.bitmap[LEFT_LEAF(index)],buddy_system.bitmap[RIGHT_LEAF(index)]);
    }
    return va;
}

void free_pages(void* va)
{
    unsigned node_size, index;
    unsigned left_node, right_node;
    
    node_size = 1;
    //初始的index是最下面一层，我要回溯到被标记为0的节点
    for (index = VA2OFFSET((uint64)va) + buddy_system.size - 1; buddy_system.bitmap[index] ; index = PARENT(index)) 
    {
        node_size *= 2;
        if (index == 0) return;
    }
    //回溯合并
    buddy_system.bitmap[index] = node_size;
    while (index) {
        index = PARENT(index);
        node_size *= 2;
        left_node = buddy_system.bitmap[LEFT_LEAF(index)];
        right_node = buddy_system.bitmap[RIGHT_LEAF(index)];
        if (left_node + right_node == node_size)
            buddy_system.bitmap[index] = node_size;
        else
            buddy_system.bitmap[index] = MAX(left_node, right_node);
    }
}
