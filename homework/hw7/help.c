#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "mlpt.h"
#include "config.h"

size_t ALMNT = 8;
size_t ENTRIES = 0;
size_t SIZE = 0;

size_t get_offset(size_t va){
    return va & (1ULL << POBITS) - 1;
}

size_t get_vpn(size_t va){
    return (va >> POBITS);
}

size_t get_index(size_t va, int lvl){
    size_t index_bits = log2(ENTRIES);
    size_t vpn = get_vpn(va);
    return vpn >> ((LEVELS - lvl)*index_bits) & (1 << index_bits) - 1;
    
}

void init_pte(pte *p, size_t va, int lvl){
    p->index = get_index(va, lvl);
    p->lvl = lvl;
    p->entry = NULL;
}

pte get_pte(size_t va, int lvl){
    if(ENTRIES == 0){
        SIZE = pow(2, POBITS);
        ENTRIES = SIZE/ALMNT;
    }
    
    size_t base = ptbr;
    pte p;
    for(int i = 0; i < lvl; i++){
        init_pte(&p, va, i+1);
        p.entry = (size_t*)base + p.index; 
        if((*p.entry & 1) == 0){
            return p;
        }
        base = *p.entry >> POBITS << POBITS;
    }
    return p;
}

size_t create_page(){
    if(ENTRIES == 0){
        SIZE = pow(2, POBITS);
        ENTRIES = SIZE/ALMNT;
    }

    void *base = NULL;    
    posix_memalign(&base, SIZE, SIZE);
    memset(base, 0, SIZE);
    return (size_t)base;
}


