#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "tools.c"

void init_va(size_t va){
    tlb_va v;
    v->id = (va >> POBITS) & 0xf;
    v->tag = va >> (POBITS + 4);
}

void init_set(bset *s,int id){
    Set s;
    s.id = id;
    s.recent = {0};
    s.addr = tlb + (id * BSIZE);
    for(int i = 0; i < 4; i++){
        way w = {0};
        s.ways[i] = w;
    }
}

int get_status(size_t t){
    int lru_s = 1;
    way c = s->recent;
    while(true){
        if (c.tag == t){
            lru_s +=1;
            return lru_s;
        }
        if(c->next->addr != 0) {
            c = c->next;
            lru_s += 1;
        }
        else return 0;
    }
}
