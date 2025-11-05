#ifndef HEAP
#define HEAP

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

void setup_brk();
void dismiss_brk();
long get_brk();
void* memory_alloc(unsigned long int bytes);
int memory_free(void *pointer);

extern unsigned long int ini_brk;
extern unsigned long int cur_brk;

#endif