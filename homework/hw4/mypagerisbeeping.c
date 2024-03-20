#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "mlpt.h"
#include "config.h"

size_t ptbr = 0;

void level_up(size_t va){
    for(int i = 0; i < LEVELS; i++){
        if ( i == 0){
            if (ptbr == 0){
                ptbr = create_page();
            }
        }
        else {
            if(i != 0){
                pte p = get_pte(va, i);
                //printf("pte in lvl 0x%zx *pte 0x%zx\n", p.entry, *p.entry);
                if (*p.entry == 0){
                    size_t pptr = create_page();
                    *p.entry = ((size_t)(pptr)) | 1; //>> POBITS) << POBITS| 1;
               }
            }
        }
    }
}

size_t translate(size_t va){
    if (ptbr == 0){
        return ~((size_t)0);
    }

    pte p = get_pte(va, LEVELS);
    
    if(p.entry == 0)
        return ~((size_t)0);
    
    if((*p.entry & 1) == 1){
        return (*p.entry >> POBITS << POBITS) | get_offset(va); 
    }
}
    
void page_allocate(size_t va){
    level_up(va);
    pte p = get_pte(va, LEVELS);
    
    if((*p.entry & 1) == 0) {
        size_t base = create_page();
        *p.entry = (size_t)(base) |1;
        //printf("in loop pa: 0x%zx pte* 0x%zx index: 0x%zu\n",p.entry, *p.entry, p.index);
    }
}
