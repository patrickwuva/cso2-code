// time to cry
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "mlpt.h"
#include "config.h"

//pageEntry;
size_t ptbr;


size_t get_offset(size_t va){
    return va & ((1UL << POBITS)-1);
}

size_t get_vpn(size_t va){
    //size_t entries = pow(2,POBITS)/ALIGNMENT;
    //size_t vpn_bits = log2(entries)*LEVELS;
    
    return (va >> POBITS) & (1UL << 9)-1;
}
pageEntry *get_pte(size_t index){
    return (pageEntry*)(ptbr+index); 
}

size_t create_page(){
    size_t base = 0;    
    void *ptr = NULL;
    posix_memalign(&ptr, 4096, 4096);
    base = (size_t)ptr;
    memset((void*)base, 0, 4096);
    return base;
}

size_t translate(size_t va){
    size_t pa = (1UL << 13) - 1;
    
    pageEntry* pte = get_pte(va);
    if((*pte & 1) == 1){
        
        return pa = *pte;
    }

    return pa;
}
    
void page_allocate(size_t va){        
    if(ptbr == 0){
        ptbr = create_page();
    }
    size_t index = get_offset(va);
    size_t vpn = get_vpn(va);
    pageEntry* pte = get_pte(vpn);

    if((*pte & 1) == 0) {
        size_t base = create_page();
        *pte = base+((0UL << POBITS)-1);
    }
}
