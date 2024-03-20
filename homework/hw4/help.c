#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mlpt.h"
#include "config.h"

size_t get_offset(size_t va){
    return va & 0xFFF;
}

size_t get_vpn(size_t va){
    return (va >> POBITS);
}

size_t get_index(size_t va, int lvl){
    size_t index_bits = log2(entries);
    size_t vpn = get_vpn(va);
    //printf("index vpn 0x%zx indexout: 0x%zx\n", vpn, vpn >> ((LEVELS - lvl)*index_bits) & (1 << index_bits) - 1);
    return vpn >> ((LEVELS - lvl)*index_bits) & (1 << index_bits) - 1;
    
}

void init_pte(pte *p, int lvl, size_t va){
    p->index = get_index(va, lvl);
    p->lvl = lvl;
}

void init_va(address *a, size_t va){
    a->va = va;
    a->vpn = get_vpn(a->va);
}

pte get_pte(int lvl, size_t va){
    size_t base = ptbr;
    //printf("base in pte 0x%zx\n", base);
    pte p;
    for(int i = 0; i < lvl; i++){
        init_pte(&p, i+1, va);
        p.entry = (size_t*)base + p.index; 
        if((*p.entry & 1) == 0){
            return p;
        }
        base = *p.entry >> POBITS << POBITS;
    }
    return p;
}
