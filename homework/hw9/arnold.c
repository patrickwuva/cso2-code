#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "tlb.h"
#include "config.h"

bset **cachemoney = NULL;

void tlb_clear(){
    if (cachemoney != NULL){
        for (int i = 0; i<16; i++){
            if (cachemoney[i] != NULL){
                for(int j = 0; j < 4; j++){
                    free(cachemoney[i]->ways[j]);
                }
                free(cachemoney[i]->ways);
                free(cachemoney[i]);
            }
        }
        free(cachemoney);
        cachemoney = NULL;
    }
}

int tlb_peek(size_t va){
    if (cachemoney == NULL){
        init_tlb(&cachemoney);
    }
    tva v;
    init_va(&v, va);
    if (cachemoney[v.id] == NULL) {
        return 0;
    }
    return get_status(cachemoney[v.id]->rway, v.tag);
}

size_t translate(size_t va){
    if (va < 0x1234000)
        return va + 0x20000;
    else if (va > 0x2000000 && va < 0x2345000)
        return va + 0x100000;
    else
        return -1;
}

size_t tlb_translate(size_t va){
    if (cachemoney == NULL){
        init_tlb(&cachemoney);
    }
    tva v;
    init_va(&v, va);
    if (cachemoney[v.id] == NULL){
        cachemoney[v.id] = init_set(cachemoney[v.id], v.id);
    }
    bset *set = cachemoney[v.id];
    way **ways = set->ways;
    for (int i = 0; i < NWAYS; i++){
        //printf("vtag 0x%zx\n",ways[i]->tag);
        if (v.tag == ways[i]->tag){
            if (ways[i] != set->rway){
            //print_lru(set->rway);
            update_recent(set->rway, get_before(set->rway, ways[i])); 
            }
        return (ways[i]->pa>>POBITS << POBITS) | v.off; 
        }
    }
    int sid = find_slot(ways);
    if (set->rway == NULL){
        set->rway = ways[0];
    }
    else{
        if (sid == -1){
            sid = get_before(set->rway,NULL)->id;
            way *b4 = get_before(set->rway, ways[sid]);
            ways[sid]->next = set->rway;
            printf("nid: %d\n", b4->next->id);
            ways[b4->id]->next = NULL;
            set->rway = ways[sid];
            printf("sid: %d b4id: %d rid: %d\n", sid, b4->id, set->rway->next->id);
        }
        else{
            ways[sid]->next = set->rway;
            set->rway = ways[sid];
        }
    }
    ways[sid]->tag = v.tag;
    ways[sid]->pa = translate(va);
    if(sid > 1){
        printf("va: 0x%zx wid: %d id: 0x%zx tag: 0x%zx rid: %d add: 0x%zx rwnid: %d\n",va, sid,v.id, ways[sid]->tag, set->rway->id,ways[sid]->pa | v.off, set->rway->next->id);
    }
    //print_lru(set->rway);
    return ways[sid]->pa | v.off; 
}











