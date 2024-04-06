#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"

void init_va(tva *v; size_t va){
    v->id = (va >> POBITS) & 0xf;
    v->tag = va >> (POBITS + 4);
}

void init_way(*way w, *tva v, size_t a, int id){
    w->tag = v->tag;
    w->next = {0};
    w->pa = a + [64 * id];
}
void init_set(bset *s, size_t a, int id){
    s->id = id;
    s->rway = {0};
    s->addr = a + (id * (BSIZE * 4);
    for(int i = 0; i < 4; i++){
        way w = {0};
        s->ways[i] = w;
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


int find_slot(way* ws){
    for (int i = 0; i < NWAYS; i++){
        if (ws[i]->addr == 0)
            return i;
    }
    return -1;
}

void update_recent(way* ws, way *rway, int id){
    way *onw = ws[id]->next;
    ws[id]->next = rway->next;
    rway->next = onw;
    rway = ws[id];
}
void update_bset(way* ws[4], way *nway, way *rway ,int id){
    nway->next = rway;    
}




