#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "tlb.h"

void test1(){
    tva v;
    init_va(&v, 0x11000);
    tlb_clear();
    assert(tlb_peek(0) == 0);
    assert(tlb_translate(0) == 0x0020000);
    assert(tlb_peek(0) == 1);
    assert(tlb_translate(0x200) == 0x20200);
    assert(tlb_translate(0x400) == 0x20400);
    assert(tlb_peek(0) == 1);
    assert(tlb_peek(0x200) == 1);
    assert(tlb_translate(0x2001200) == 0x2101200);
    assert(tlb_translate(0x0005200) == 0x0025200);
    assert(tlb_translate(0x0008200) == 0x0028200);
    assert(tlb_translate(0x0002200) == 0x0022200);
    assert(tlb_peek(0x2001000) == 1);
    assert(tlb_peek(0x0001000) == 0);
    assert(tlb_peek(0x0004000) == 0);
    assert(tlb_peek(0x0005000) == 1);
    assert(tlb_peek(0x0008000) == 1);
    assert(tlb_peek(0x0002000) == 1);
    assert(tlb_peek(0x0000000) == 1);
    tlb_clear();
    assert(tlb_peek(0x2001000) == 0);
    assert(tlb_peek(0x0005000) == 0);
    assert(tlb_peek(0x0008000) == 0);
    assert(tlb_peek(0x0002000) == 0);
    assert(tlb_peek(0x0000000) == 0);
    assert(tlb_translate(0) == 0x20000);
    assert(tlb_peek(0) == 1);
}

void test2(){
    tlb_clear();
    assert(tlb_translate(0x0001200) == 0x0021200);
    assert(tlb_translate(0x2101200) == 0x2201200);
    assert(tlb_translate(0x0801200) == 0x0821200);
    assert(tlb_translate(0x2301200) == 0x2401200);
    assert(tlb_translate(0x0501200) == 0x0521200);
    assert(tlb_translate(0x0A01200) == 0x0A21200);
    assert(tlb_peek(0x1200) == 0);
    assert(tlb_peek(0x2101200) == 0);
    printf("peek %d\n", tlb_peek(0x2301200));
    assert(tlb_peek(0x2301200) == 3);
    assert(tlb_peek(0x0501200) == 2);
    assert(tlb_peek(0x0801200) == 4);
    assert(tlb_peek(0x0A01200) == 1);
    assert(tlb_translate(0x2301800) == 0x2401800);
    assert(tlb_peek(0x0001000) == 0);
    assert(tlb_peek(0x2101000) == 0);
    assert(tlb_peek(0x2301000) == 1);
    assert(tlb_peek(0x0501000) == 3);
    assert(tlb_peek(0x0801000) == 4);
    assert(tlb_peek(0x0A01000) == 2);
    assert(tlb_translate(0x404000) == 0x424000);
    tlb_clear();
    assert(tlb_peek(0x301000) == 0);
    assert(tlb_peek(0x501000) == 0);
    assert(tlb_peek(0x801000) == 0);
    assert(tlb_peek(0xA01000) == 0);
    assert(tlb_translate(0xA01200) == 0xA21200);
}
int main (){
    test2(); 
    return 0;
}

