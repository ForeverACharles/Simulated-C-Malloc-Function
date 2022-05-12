#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#endif

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

void* mymalloc(size_t size, char* fileName, int line);
void myfree(void* mem_addr, char* fileName, int line);