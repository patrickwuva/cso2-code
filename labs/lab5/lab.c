#define _GNU_SOURCE
#include "util.h"
#include <stdio.h>      // for printf
#include <stdlib.h>     // for atoi (and malloc() which you'll likely use)
#include <sys/mman.h>   // for mmap() which you'll likely use
#include <stdalign.h>

alignas(4096) volatile char global_array[4096 * 32];

void one(){
    int p = 4095;
    
    char s[] = "hello";
    int o = 100;
    
    for(int i = 0; i < sizeof(s); i++){
        global_array[o+i] = s[i]; 
    }
    for(int i = 0; i < sizeof(s); i++){
        global_array[(p*5)+i] = s[i];           
    }
    o = 50;
    for(int i = 0; i < sizeof(s); i++){
        global_array[o+i] = s[i]; 
    }


}

void two(){
    char* arr = (char*) malloc(1000000);
    
    char* s = "hello";

    for(int i = 0; i < sizeof(s); i++){
        arr[i+10] = s[i];

    }
}

void three(){
    char* arr = (char*) malloc(1048576-4096);
    char* s = "h";

    for(int i = 0; i < 32; i++){
        for(int j = 0; j < sizeof(s); j++){
            arr[j+(i*4097)] = s[j];
        }
    }
}

void four(){
    //55555555a000-5555555bbfff 
    char *ptr;
    ptr = mmap((void*) 0x5555555bbfff+0x200000,
           4092,
           PROT_READ | PROT_WRITE,
           MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
           -1,
            0
    );
    //if (ptr == MAP_FAILED) { handle_error(); } 
}
void labStuff(int which) {
    if (which == 0) {
        /* do nothing */
    } else if (which == 1) {
        one();
    } else if (which == 2) {
        two();
    } else if (which == 3) {
        three();
    } else if (which == 4) {
        four();
    }
}

int main(int argc, char **argv) {
    int which = 0;
    if (argc > 1) {
        which = atoi(argv[1]);
    } else {
        fprintf(stderr, "Usage: %s NUMBER\n", argv[0]);
        return 1;
    }
    printf("Memory layout:\n");
    print_maps(stdout);
    printf("\n");
    printf("Initial state:\n");
    force_load();
    struct memory_record r1, r2;
    record_memory_record(&r1);
    print_memory_record(stdout, NULL, &r1);
    printf("---\n");

    printf("Running labStuff(%d)...\n", which);

    labStuff(which);

    printf("---\n");
    printf("Afterwards:\n");
    record_memory_record(&r2);
    print_memory_record(stdout, &r1, &r2);
    print_maps(stdout);
    return 0;
}
