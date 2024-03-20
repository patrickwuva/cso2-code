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

typedef struct {
    size_t* entry;
    int lvl;
    size_t index;

} pte;

typedef struct {
    size_t va;
    size_t vpn;
} address;

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
        p.entry = (size_t*)(base) + p.index; 
        if(i < lvl - 1){
            if((*p.entry & 1) == 0){
                return p;
            }
        }
        base = *p.entry & ~((1ULL << POBITS) - 1); 
        //base = *p.entry >> POBITS << POBITS;
    }
    return p;
}
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
                //pte p;
                //init_pte(&p, 1, va);
                //p.entry = (size_t*)ptbr + p.index;
                //printf("entry in lvlup 0x%zx\n",p.entry);
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
    //printf("pa: 0x%zx pte* 0x%zx index: 0x%zu\n",p.entry, *p.entry, p.index);
    if((*p.entry & 1) == 0) {
        size_t base = create_page();
        *p.entry = (size_t)(base) |1;
        printf("in loop pa: 0x%zx pte* 0x%zx index: 0x%zu\n",p.entry, *p.entry, p.index);
    }
}
