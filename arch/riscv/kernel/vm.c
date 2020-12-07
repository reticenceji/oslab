#include "vm.h"

static void add_entry(uint64 page_addr,uint64 entry_addr,int number,int permisson);
static frame_queue_t *fq;

int fq_is_full(frame_queue_t *fq)
{
    return fq->front == (fq->rear+1) % fq->capacity;
}

int fq_is_empty(frame_queue_t *fq)
{
    return fq->front == fq->rear;
}

void init_frame_queue(frame_queue_t *fq)
{
    fq->front = 0;
    fq->capacity = TEST_FRAME_NUM + 1;
    // TODO the elements in fq->frame haven't been initialized yet
    for (fq->rear=0;fq->rear<TEST_FRAME_NUM;fq->rear++)
        fq->frame[fq->rear] = KERNEL_START_P + FRAME_SIZE*fq->rear;
}

uint64 alloc_frame(frame_queue_t *fq)
{
    if (fq_is_empty(fq))
    {
        return NULL;
    }
    uint64 valid_frame = fq->frame[fq->front];
    fq->front = (fq->front+1) % fq->capacity;
    return valid_frame;     
}

void free_frame(frame_queue_t *fq)
{
    if (fq_is_full(fq))
    {
        return;
    }
    fq->frame[fq->rear] = fq->frame[(fq->front-1) % fq->capacity];
    fq->rear = (fq->rear+1) % fq->capacity;
}

uint64* paging_init()
{
    uint64* page_base;
    init_frame_queue(fq);
    page_base = (uint64*)alloc_frame(fq);
    //映射到高地址,没有关注权限位
    create_mapping(page_base,KERNEL_START_V,KERNEL_START_P,KERNEL_SIZE,FLAG_U|FLAG_R|FLAG_W|FLAG_X);
    //等值映射,先留着也许可以不用
    create_mapping(page_base,KERNEL_START_V,KERNEL_START_P,KERNEL_SIZE,FLAG_U|FLAG_R|FLAG_W|FLAG_X);
    //低地址的等值映射,可以理解为是那些外部设备map到内存的地方.
    create_mapping(page_base,UART_START,UART_START,UART_SIZE, FLAG_U|FLAG_R|FLAG_W|FLAG_X);
    return page_base;
}

int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
{
    // TODO 
    /* 分别是页表数量，三级页表数量，二级页表数量 */
    int num_pd,num_pmd,num_pud; 
    int i,j,k;
    uint64 addr_pud,addr_pmd,addr_pd;
    num_pd = (sz+PAGE_SIZE-1) << 12;
    num_pmd = (num_pd+ENTRY_PER_PAGE-1) << 9;
    num_pud = (num_pd+ENTRY_PER_PAGE-1) << 9;
    for (i=0;i<num_pud;i++)
    {
        addr_pud = alloc_frame(fq);
        add_entry((uint64)pgtbl,addr_pud,i,perm);
        for (j=0;j<num_pmd;j++)
        {
            addr_pmd = alloc_frame(fq);
            add_entry(addr_pud,addr_pmd,j,perm);
            for (k=0;k<num_pd;k++)
            {
                addr_pd = alloc_frame(fq);
                add_entry(addr_pmd,addr_pd,j,perm);
            }
        }
    }
}

static void add_entry(uint64 page_addr,uint64 entry_addr,int number,int permisson)
{
    *(uint64 *)(page_addr + (uint64)number*8) = (entry_addr >> 12 << 12) + permisson;
} 