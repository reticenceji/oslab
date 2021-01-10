#include "buddy.h"
#include "types.h"
#include "vm.h"

#define LEFT_LEAF(INDEX) (((INDEX)<<1)+1)
#define RIGHT_LEAF(INDEX) (((INDEX)<<1)+2)
#define PARENT(INDEX) (((INDEX)-1)>>1)
#define MAX(A,B) ((A)>(B)?(A):(B))
#define OFFSET2VA(OFFSET) (((OFFSET)<<12)+KERNEL_START_V)
#define VA2OFFSET(VA) (((VA)>>12)-KERNEL_START_V)

extern uint64 _start ;
extern uint64 _end;
static struct buddy buddy_system;
static inline fixsize(int size);

/* 把size对齐到2^n */
static inline fixsize(int size)
{
    int fix_size=1;
    size--;
    while (size)
    {
        fix_size<<=1;
        size>>=1;
    }
}

// static int find_index(int index,int num)
// {
//     if (buddy_system.bitmap[index]>=num)
//     {
//         if (buddy_system.bitmap[2*index+1]>=num) return find_index(2*index+1,num);
//         if (buddy_system.bitmap[2*index+2]>=num) return find_index(2*index+1,num);
//         return index;
//     }   
//     else 
//         return -1;
// }

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
    init_bitmap(0,buddy_system.size);

    alloc_pages((uint64)&_end-(uint64)&_start);
}

// TODO
void * alloc_pages(int size) {
    unsigned index = 0;
    unsigned node_size;
    uint64 va;

    size = fixsize(size);
    /* 如果小于的话说明空间不够了 */
    if (buddy_system.bitmap[index] < size)
        return -1;

    /* 在bitmap里寻找合适的index */
    for(node_size = buddy_system.size; node_size != size; node_size /= 2) {
        if (buddy_system.bitmap[LEFT_LEAF(index)] >= size)
            index = LEFT_LEAF(index);
        else
            index = RIGHT_LEAF(index);
    }
    /* 算出在大页表里的偏移 */
    va = OFFSET2VA( (index + 1) * node_size - buddy_system.size);
    
    buddy_system.bitmap[index] = 0;
    //返回修改父节点的值
    while (index) {
        index = PARENT(index);
        buddy_system.bitmap[index] = MAX(buddy_system.bitmap[LEFT_LEAF(index)],buddy_system.bitmap[RIGHT_LEAF(index)]);
    }

    return va;
}

// TODO
void free_pages(void*);

void buddy2_free(int va) {
    unsigned node_size, index;
    unsigned left_node, right_node;
    
    node_size = 1;
    //找到被标记为0的那个节点
    for (index = VA2OFFSET(va) + buddy_system.size - 1; buddy_system.bitmap[index] ; index = PARENT(index)) 
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