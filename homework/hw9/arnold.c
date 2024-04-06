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
    tlb.tp= (size_t)base;
    tlb.sets = {0};
}

void tlb_clear(){
    memset(base, 0, SIZE);
}

int tlb_peek(size_t va){
    if (tlb.pt == 0) {
        create_tlb();
        return 0;
    }
    tlb_va v = init_va(va);
    if (tlb.sets[v.id]->addr == 0) return 0;
    bset *curr_set = tlb.sets[v.id];
    for (int i = 0; i < NWAYS; i++){
        if (curr_set->ways[i]->tag == v.tag){
            return get_status(v.tag);
        }
    }
    return 0;
}

size_t tlb_translate(size_t va){
    if (tlb.tp == 0) create_tlb();
    tva v;
    init_va(&v, va)
    bset s;
    if (tlb.sets[v.id]->addr == 0){
        init_set(&s, v.id);
    }
    way *ws = s->ways;
    for (int i = 0; i < NWAYS; i++){
        if (v.tag == ws[i]->tag){
            update_recent(ws, rway,i); 
            return get_status(ws[i]->tag);
        }
    }
    way nw;
    int oid = find_slot(ws);
    if (oid == -1){
        oid = get_last(ws, s->rway)
        init_way(&nw, &v, s.addr)
        
    }
    int lid = get_last(ws, &s->rway);
    if (oid == -1){
        way *lw;
        
    }
}











