// time to cry
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "mlpt.h"
#include "config.h"
#include <math.h>

size_t size = 4096;
size_t alignment = 8;

size_t ptbr = 0;


void map(va){}
void page_allocate(size_t va){        
    void *ptr = NULL;
    posix_memalign(&ptr, alignment, size);
    ptbr = (size_t)ptr;
    memset((void*)ptbr, 0, size);
}

int main(){
    assert(ptbr == 0); 
    page_allocate(0);
    printf("base: 0x%lx\n",ptbr);
    int index = 0xbc;
    pageEntry* pt = (pageEntry*)ptbr;
    pageEntry e = pt[0xbc];
    e = 0x1234;
    printf("chunk: 0x%llX\n",e);
    
    return 0;
}
