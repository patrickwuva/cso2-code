#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "config.h"


size_t testva = 0x456789abcdef;
int main(){
    assert(ptbr == 0); 
    page_allocate(testva);

    int *p1 = (int *)translate(testva);

    printf("physical adrr %d\n", *p1);
    return 0;
}
