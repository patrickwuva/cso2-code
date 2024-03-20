#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "config.h"
#include "mlpt.h"
#include <stdalign.h>
int pages_created;
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
    page_allocate(va2);
    printf("translate: 0x%zx\n",translate(va1));
    printf("translate: 0x%zx\n",translate(va2));
    printf("ptrb 0x%zx pages_created %d\n", ptbr, pages_created);

}
void lvl2test(){
    size_t va0 = 0x0;
    size_t va1 = 0x1000;
    size_t va2 = 0x200000;
    size_t va3 = 0x400000;
    size_t va4 = 0x1ff000;
    size_t va5 = 0x5ff000;
    page_allocate(va0);
    page_allocate(va2);
    printf("translate: 0x%zx\n",translate(va0));
    printf("translate: 0x%zx\n",translate(va2));
    page_allocate(va0);
    printf("translate: 0x%zx\n",translate(va0));
    printf("done pages created: %d\n",pages_created);
}

void crazy_test(){
    assert(ptbr == 0);
    page_allocate(0x456789abcdef);
    assert(ptbr != 0);
    printf("translate: 0x%zx\n", translate(0x456789abcd00));
    printf("translate: 0x%zx\n", translate(0x45B7A9abcd02));
    page_allocate(0x456789ab0000);
    printf("translate: 0x%zx\n", translate(0x456789ab0000));
    page_allocate(0x456780000000);
}

void lvl3(){
    size_t va0 = 0x20400;
    page_allocate(va0);
    page_allocate(va0);
    printf("t: 0x%zx\n",translate(va0));
    printf("done\n");
}
int main(){
    lvl2test();

    return 0;
}
