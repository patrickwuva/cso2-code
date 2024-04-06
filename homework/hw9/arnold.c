#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "tools.c"

int SIZE = pow(2,POBITS);
Tlb tlb;
tlb.tlbptr = 0;

void create_tlb(){
    void *base = NULL;
    posix_memalign(&base, SIZE, SIZE);
    memset(base, 0, SIZE);
    tlb.tlbptr = (size_t)base;
    tlb.sets = {0};
}

void tlb_clear(){
    memset(base, 0, SIZE);
}

int tlb_peek(size_t va){
    if (tlb.tlbptr == 0) {
        create_tlb();
        return 0;
    }
    tlb_va v = init_va(va);
    if (tlb.sets[sid].addr == 0) return 0;
    bset *curr_set = tlb.sets[v.id];
    for (int i = 0; i < NWAYS; i++){
        if (curr_set->ways[i]->tag == v.tag){
            return get_status(v.tag);
        }
    }
    return 0;
}

size_t tlb_translate(size_t va){
    if (tlb.tlbptr == 0) create_tlb();
     
    
}
