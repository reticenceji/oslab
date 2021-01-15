#ifndef _BUDDY_H
#define _BUDDY_H

#include "types.h"

//它叫bitmap，但不是bitmap。存储的是longest
struct buddy {
	unsigned long size;
	unsigned *bitmap; 
};

void init_buddy_system(void);
void *alloc_pages(int);
void free_pages(void*);

#endif