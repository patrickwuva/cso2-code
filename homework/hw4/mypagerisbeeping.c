#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "mlpt.h"
#include "config.h"

int pages_created = 0;
size_t ptbr = 0;
size_t entries = 0;
size_t almnt = 8;
size_t size = 0;
size_t get_offset(size_t va){
    return va & 0xFFF;
}

size_t get_vpn(size_t va){
    return (va >> POBITS);
}

size_t get_lvl_index(size_t vpn, int lvl){
    size_t index_bits = log2(entries);
    return vpn >> ((LEVELS - lvl)*index_bits) & (1 << index_bits) - 1;
}

size_t* get_pte(size_t va, int lvl){
    size_t vpn = get_vpn(va);
    size_t base = ptbr;
    size_t *pte;
    for(int i = 0; i < lvl; i++){
        size_t index = get_lvl_index(vpn, i+1);
        printf("b: 0x%zx i: 0x%zx pb: 0x%zx in: %d\n", base, index, ptbr, i);
        if(i < lvl - 1){
            if((*pte & 1) == 0){
                return 0;
            }
            base = *pte & ~((1ULL << POBITS) - 1);
        }
    }
    return pte;
}

size_t create_page(){
    void *base = NULL;    
    if( posix_memalign(&base, size, size) != 0){
        printf("what?\n");
    }
    memset(base, 0,size);
    pages_created += 1;
    return (size_t)base;
}

void level_up(size_t va){
    for(int i = 0; i < LEVELS; i++){
        if ( i == 0){
            if (ptbr == 0){
                ptbr = create_page();
            }
        }
        else {
            if(i != 0){
                size_t *pte = get_pte(va, i);
                if (*pte == 0){
                    size_t pptr = create_page();
                    *pte = ((size_t)(pptr)) | 1; //>> POBITS) << POBITS| 1;
               }
            }
        }
    }
}
size_t translate(size_t va){
    size_t *pte = get_pte(va, LEVELS);
    if(pte == 0)
        return ~((size_t)0);
    if((*pte & 1) == 1){
        return (*pte >> POBITS << POBITS) | get_offset(va); 
    }

    return ~((size_t)0);
}
    
void page_allocate(size_t va){
    if(entries == 0){
        size = pow(2, POBITS);
        entries = size/almnt;
    }
    level_up(va);
    size_t *pte = get_pte(va, LEVELS);
    if((*pte & 1) == 0) {
        size_t base = create_page();
        *pte = (size_t)(base) |1;
    }
}
