#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "mlpt.h"
#include "config.h"

size_t ptbr = 0;
size_t almnt = 8;
size_t entries = 0;
size_t size = 0;

size_t create_page(){
    void *base = NULL;    
    if( posix_memalign(&base, size, size) != 0){
    }
    memset(base, 0,size);
    return (size_t)base;
}

void level_up(size_t va){
    for(int i = 0; i < LEVELS; i++){
        if ( i == 0){
            if (ptbr == 0){
                ptbr = create_page();
                pte p;
                init_pte(&p, 1, va);
                p.entry = (size_t*)ptbr + p.index;
                printf("entry in lvlup 0x%zx\n",p.entry);
            }
        }
        else {
            if(i != 0){
                pte p = get_pte(i, va);
                if (p.entry == NULL){
                    size_t pptr = create_page();
                    *p.entry = (size_t)pptr | 1; //>> POBITS) << POBITS| 1;
               }
            }
        }
    }
}
size_t translate(size_t va){
    if(entries == 0){
        size = pow(2, POBITS);
        entries = size/almnt;
    }
    pte p = get_pte(LEVELS, va);
    //printf("T pte: 0x%zx *pte 0x%zx index 0x%zx\n", p.entry, *p.entry, p.index);
    if(p.entry == NULL)
        return ~((size_t)0);
    if((*p.entry & 1) == 1){
        return (*p.entry >> POBITS << POBITS) | get_offset(va); 
    }
    
    return ~((size_t)0);
}
    
void page_allocate(size_t va){
    if(entries == 0){
        size = pow(2, POBITS);
        entries = size/almnt;
    }
    level_up(va);
    pte p = get_pte(LEVELS, va);
    printf("pa: 0x%zx pte* 0x%zx index: 0x%zu\n",p.entry, *p.entry, p.index);
    if(((size_t)p.entry & 1) == 0) {
        size_t base = create_page();
        *p.entry = (size_t)(base) |1;
        //printf("in loop pa: 0x%zx pte* 0x%zx index: 0x%zu\n",p.entry, *p.entry, p.index);
    }
}
