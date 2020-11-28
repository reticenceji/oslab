#include "vm.h"

extern frame_queue_t *fq;

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
    fq->rear = 0;
    fq->capacity = TEST_FRAME_NUM + 1;
    // the elements in fq->frame haven't been initialized yet
}

frame_t *alloc_frame(frame_queue_t *fq)
{
    if (fq_is_empty(fq))
    {
        return NULL;
    }
    frame_t *valid_frame = fq->frame[fq->front];
    fq->front = (fq->front+1) % fq->capacity;
    return valid_frame;     //返回值valid_frame是一个数组指针, 指向一块4KB的物理内存
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
