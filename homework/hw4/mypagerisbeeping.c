#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "mlpt.h"
#include "config.h"

size_t ptbr = 0;
size_t entries = 0;
size_t va_bits = LEVELS*9;
size_t almnt = 8;
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
        pte = (size_t*)base + index;
        if(i != lvl - 1 && *pte == 0){
            //printf("uhoh\n");
            return 0;
        }
        //printf("i: %d pte: 0x%zx index: 0x%zx\n", i, *pte, index);
        base = (*pte >> POBITS) << POBITS;
    }
    return pte;
}

size_t create_page(){
    void *base = NULL;    
    posix_memalign(&base, 4096, 4096);
    memset(base, 0, 4096);
    return (size_t)base;
}

void level_up(size_t va){
    for(int i = 0; i < LEVELS; i++){
        size_t pptr = create_page();
        if ( i == 0 && ptbr == 0){
            ptbr = pptr;
        }
        else{
            if(i != 0){
                size_t *pte = get_pte(va, i);
                *pte = ((size_t)(pptr) >> POBITS) << POBITS| 1;
            }
        }
    }
}
size_t translate(size_t va){
    //printf("lvlindex: 0x%zx\n", get_lvl_index(get_vpn(va), 1));
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
        entries = pow(2,POBITS)/almnt;
    }
    /*if(ptbr == 0){
        level_up(va);
    }*/
    level_up(va);
    size_t *pte = get_pte(va, LEVELS);
    if((*pte & 1) == 0) {
        size_t base = create_page();
        *pte = ((size_t)(base)>> POBITS) << POBITS |1;
        //printf("va: 0x%zx ptbr: 0x%zx pte: 0x%zx pteaddr: 0x%zx  prevaddr: 0x%zx\n",va, ptbr, *pte, pte,get_pte(va,1));
    }
    //printf("pa done. pte: 0x%zx pteaddr: 0x%zx ptbr: 0x%zx\n", *pte, pte, ptbr);
}
