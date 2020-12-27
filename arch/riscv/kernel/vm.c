#include "vm.h"
#include "put.h"

extern uint64 text_start;
extern uint64 text_end;
extern uint64 rodata_start;
extern uint64 rodata_end;
extern uint64 data_start;
extern uint64 data_end;
extern uint64 bss_start;
extern uint64 bss_end;
extern uint64 other_start;
extern uint64 _end;

/* 判断队列是否为满, 为满返回1, 否则返回0 */
static int fq_is_full(frame_queue_t *fq);
/* 添加一个页表项 */
static void add_entry(uint64 page_addr, int vpn, uint64 ppn, int perm);
/* 物理页管理队列 */
static frame_queue_t frame_queue;

static void add_entry(uint64 page_addr, int vpn, uint64 ppn, int perm)
{
    ((uint64 *)page_addr)[vpn] = ((ppn >> 12) << 10) + perm;
    #ifdef DEBUGTEST
        print("\t[info]Page Table Entry: %X[%X]:%X\n", page_addr, vpn, ((ppn>>12) << 10)+perm);
    #endif
    return;
} 

static int fq_is_full(frame_queue_t *fq)
{
    return fq->front == (fq->rear+1) % fq->capacity;
}

static int fq_is_empty(frame_queue_t *fq)
{
    return fq->front == fq->rear;
}

void init_frame_queue()
{
    frame_queue_t *fq = &frame_queue;
    fq->front = 0;
    fq->capacity = TEST_FRAME_NUM + 1;
    for (fq->rear=0;fq->rear<TEST_FRAME_NUM;fq->rear++)
        fq->frame[fq->rear] = KERNEL_START_P + KERNEL_SIZE + FRAME_SIZE*fq->rear;
}

uint64 alloc_frame()
{
    frame_queue_t *fq = &frame_queue;
    if (fq_is_empty(fq))
    {
        return NULL;
    }
    uint64 valid_frame = fq->frame[fq->front];
    fq->front = (fq->front+1) % fq->capacity;
    return valid_frame;     
}

void free_frame()
{
    frame_queue_t *fq = &frame_queue;
    if (fq_is_full(fq))
    {
        return;
    }
    fq->frame[fq->rear] = fq->frame[(fq->front-1) % fq->capacity];
    fq->rear = (fq->rear+1) % fq->capacity;
}

__attribute__((optimize("O0"))) uint64* paging_init()
{
    #ifdef DEBUGTEST
    print("[*]\tFunction paging_init\n[info] frame_queue:%X\n",&frame_queue);
    #endif

    uint64* pgtbl;
    init_frame_queue();
    pgtbl = (uint64*)alloc_frame();
    kernel_mapping(pgtbl); 

    #ifdef DEBUGTEST
    print("[*]\tFunction paging_init DONE\n");  
    #endif
    return pgtbl;
}

__attribute__((optimize("O0"))) void kernel_mapping(uint64 *pgtbl)
{ 
    //映射到高地址,没有关注权限位
    //create_mapping(pgtbl, KERNEL_START_V, KERNEL_START_P, KERNEL_SIZE, FLAG_R|FLAG_W|FLAG_X|FLAG_V);
    
    //text段映射, r-x
    create_mapping(pgtbl, ((uint64)&text_start) + MAP_OFFSET, KERNEL_START_P, (uint64)&text_end - KERNEL_START_P, FLAG_R|FLAG_X|FLAG_V);

    //rodata段映射, r--
    create_mapping(pgtbl, ((uint64)&rodata_start) + MAP_OFFSET, ((uint64)&rodata_start), (uint64)&rodata_end - ((uint64)&rodata_start), FLAG_R|FLAG_V);

    //data段映射, rw-
    create_mapping(pgtbl, ((uint64)&data_start) + MAP_OFFSET, ((uint64)&data_start), (uint64)&data_end - ((uint64)&data_start), FLAG_R|FLAG_W|FLAG_V);

    //bss段映射, rw-
    create_mapping(pgtbl, ((uint64)&bss_start) + MAP_OFFSET, ((uint64)&bss_start), (uint64)&bss_end - ((uint64)&bss_start), FLAG_R|FLAG_W|FLAG_V);

    //other映射, rw-
    create_mapping(pgtbl, (uint64)&other_start + MAP_OFFSET, ((uint64)&other_start), KERNEL_SIZE-((uint64)&other_start-(uint64)&text_start), FLAG_R|FLAG_W|FLAG_V);

    //把Kernel剩下的allocable空间也都映射了
    create_mapping(pgtbl, (uint64)&_end + MAP_OFFSET, ((uint64)&_end), KERNEL_ALLOCABLE_SIZE, FLAG_R|FLAG_W|FLAG_V);

    //UART的映射,可以理解为是那些外部设备map到内存的地方.
    create_mapping(pgtbl, UART_START_V, UART_START_P, UART_SIZE, FLAG_R|FLAG_W|FLAG_X|FLAG_V);
}

__attribute__((optimize("O0"))) void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
{
    /* 分别是页表条目数量，三级页表数量，二级页表数量 */
    int num_pd, num_pmd, num_pud;
    int flag=0;   //Flag表示我们现在是否开启了页表
    /* 最后一张二级页表可能不能被三级页表填满，最后一张三级页表可能不能被entry填满 */
    int jmax, kmax;
    uint64 addr_pd, addr_pmd, addr_pud;
    int i, j, k;
    int vpn2, vpn1, vpn0;

    num_pd = (sz + PAGE_SIZE-1) >> 12;
    num_pmd = (num_pd + ENTRY_PER_PAGE-1) >> 9;
    num_pud = (num_pmd + ENTRY_PER_PAGE-1) >> 9;
    #ifdef DEBUGTEST
        print("[*] Function create_mapping START\n");
        print("pgtbl=%X, va=%X, pa=%X\n", pgtbl, va, pa);
        print("[info]\tsz=%X, pud=%X, pmd=%X, pd=%X\n", sz, num_pud, num_pmd, num_pd);
    #endif
    flag = __is_page_open();
    if (flag==1) pgtbl = pgtbl+MAP_OFFSET;
    for (i = 0; i < num_pud && num_pud >= 0; i++)
    {
        vpn2 = (va >> 30) & 0x1FF;
        if (pgtbl[vpn2] == 0)
        {
            addr_pud = alloc_frame();
            add_entry((uint64)pgtbl, vpn2, addr_pud, FLAG_V);
        }
        else
        {
            addr_pud = (pgtbl[vpn2] >> 10) << 12;
        }
        jmax = (i == num_pud-1) ? ((num_pmd-1) & 0x1FF)+1 : ENTRY_PER_PAGE;
        if (flag==1) addr_pud=VP(addr_pud);

        for (j = 0; j < jmax && jmax >= 0; j++)
        {
            vpn1 = (va >> 21) & 0x1FF;
            if ( ((uint64 *)addr_pud)[vpn1] == 0 )
            {
                addr_pmd = alloc_frame();
                add_entry(addr_pud, vpn1, addr_pmd, FLAG_V);
            }
            else
            {
                addr_pmd = (((uint64 *)addr_pud)[vpn1] >> 10) << 12;
            }
            kmax = (i == num_pud-1 && j == jmax-1) ? ((num_pd-1) & 0x1FF)+1 : ENTRY_PER_PAGE;
            if (flag==1) addr_pmd=VP(addr_pmd);

            for (k = 0; k < kmax && kmax >= 0; k++)
            {
                vpn0 = (va >> 12) & 0x1FF;
                if ( ((uint64 *)addr_pmd)[vpn0] == 0 )
                {
                    add_entry(addr_pmd, vpn0, pa, perm);
                }
                else
                {
                    kmax--;
                }
                va += PAGE_SIZE;
                pa += FRAME_SIZE;
            }
        }
    }
    #ifdef DEBUGTEST
        print("[*]Done create_mapping\n");  
    #endif
    return;
}
