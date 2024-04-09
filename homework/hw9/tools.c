#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "config.h"
void init_va(tva *v, size_t va){
    v->id = (va >> POBITS) & 0xf;
    v->tag = va >> (POBITS + 4);
    v->off = va & 0xFFF;
}
bset* init_set(bset *set, int id){
    set = malloc(sizeof(bset));
    set->id = id;
    set->ways = malloc(4 * sizeof(way *));
    for(int i = 0; i < 4; i++){
        set->ways[i] = malloc(sizeof(way));
        set->ways[i]->id = i;
        set->ways[i]->tag = -1;
        set->ways[i]->pa = 0;
        set->ways[i]->next = NULL;
    }
    set->rway = NULL; 
    return set;
}

void init_tlb(bset ***cache){
    *cache = calloc(16, sizeof(bset *));
}

int get_status(way *rway, size_t tag){
    int wstat = 1;
    way *c = rway;
    while(c != NULL){
        if (c->tag == tag){
            return wstat;
        }
        c = c->next;
        wstat +=1;
    }
    return 0;
}
int find_slot(way **ways){
    for (int i = 0; i < NWAYS; i++){
        if (ways[i]->pa == 0 ){
            return i;
        }
    }
    return -1;
}
void update_recent(way *rway, way *before){
    way *tway = before->next;
    before->next = rway;
    rway = tway;
    
}
way *get_before(way *rway, way *nway){
    if(rway->next == NULL){
        return rway;
    }
    way *c = rway;
    while (c != NULL){
        if(c->next == nway){
            return c;
        }
        c = c->next;
    }
}

void print_lru(way *rway){
    printf("[id: %d]",rway->id);
    if (rway->next != NULL){
        while(1){
            way *c = rway->next;
            printf("->[id: %d]",c->id);
            if(c->next == NULL){
                break;
            }
            c = c->next;
        }
    }
    printf("\n");
}




