#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "lru.c"

int SIZE = pow(2,POBITS);
Tlb tlb;
tlb.tlbptr = 0;
Set init_set(int id){
    Set s;
    s.id = id;
    s.addr = tlb + (id * BSIZE);
    for(int i = 0; i < 4; i++){
        Way w;
        w.tag = 0;
        w.addr = 0;
        w.next = {0};
        s.ways[i] = w;
    }
}

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
    int index = (va >> POBITS) & 0xf;
    int tag = va >> (POBITS + 4);
    
    if (index >= NSETS) return 0;
    if (tlb.sets[index].addr == 0) return 0;
    
    bset curr_set = tlb.sets[index];
    for (int i = 0; i < NWAYS; i++){
        if (curr_set.ways[i].tag == tag) ;
    }
    
    return va;
}

size_t tlb_translate(size_t va){
    if (tlb.tlbptr == 0) create_tlb();

    
}
