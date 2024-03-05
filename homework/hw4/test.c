#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "config.h"
#include "mlpt.h"

void mantest(){
    page_allocate(0x0);
    size_t *pointer_to_table;
    pointer_to_table = (size_t *) ptbr;
    size_t page_table_entry = pointer_to_table[0];
    printf("PTE @ index 3: valid bit=%d  physical page number=0x%lx ptbr: 0x%lx\n",
    (int) (page_table_entry & 1),
    (long) (page_table_entry >> 12),
    (long) (ptbr)
    );
}

void lvl1test(){
    size_t va1 = 0x0;
    size_t va2 = 0x1000;
    assert(ptbr == 0); 
    page_allocate(va1);
    //printf("translate: 0x%zx\n",translate(va1));
    page_allocate(va2);
    printf("translate: 0x%zx\n",translate(va1));
    printf("translate: 0x%zx\n",translate(va2));
    printf("ptfd 0x%zx\n", ptbr);
    //page_allocate(0x22000);:a
    //printf("translate: 0x%zx\n",translate(va2));

}
void lvl2test(){
    assert(ptbr == 0);
    size_t va1 = 0x0;
    size_t va2 = 0x200000;
    size_t va0 = 0x1000;
    size_t va3 = 0x300000;
    page_allocate(va0);
    printf("translate: 0x%zx\n",translate(va0));
    //printf("translate: 0x%zx\n",translate(va0));
    //page_allocate(va3); 
   // printf("translate: 0x%zx\n",translate(va3));
}

void crazy_test(){
    // 0 pages have been allocated
    assert(ptbr == 0);

    page_allocate(0x456789abcdef);
    // 5 pages have been allocated: 4 page tables and 1 data
    assert(ptbr != 0);

    //page_allocate(0x456789abcd00);
    // no new pages allocated (still 5)
    
    printf("translate: 0x%zx\n", translate(0x456789abcd00));
    printf("translate: 0x%zx\n", translate(0x45B7A9abcd02));

    //assert(translate(0x456789ab0000) == 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456789ab0000);
    // 1 new page allocated (now 6; 4 page table, 2 data)

    //assert(translate(0x456789ab0000) != 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456780000000);
    // 2 new pages allocated (now 8; 5 page table, 3 data)

}
int main(){
    //lvl2test();
    lvl1test();
    ////printf("\n\n test 2\n");
    mantest();
    return 0;
}
