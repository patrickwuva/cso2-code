#define _XOPEN_SOURCE 700
#include "gettimings.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct timespec s, e;
long long nsecs(){
    return ((e.tv_sec - s.tv_sec)*1000000000) + (e.tv_nsec - s.tv_nsec);
}

long long get_overhead(){
    long long sum = 0;
    int total = 1000000;
    for(int i = 0; i<total; i++){
        
        clock_gettime(CLOCK_MONOTONIC, &s);
        clock_gettime(CLOCK_MONOTONIC, &e);
        sum += nsecs();
    }

    return sum/total;
}

void lets_time(void (*f)()){
    long long overhead = get_overhead();
    
    printf("overhead: %lld\n",overhead);
    int total = 100000;
    long long sum = 0; 
    for(int i = 0; i < total; i++){
        clock_gettime(CLOCK_MONOTONIC, &s);
        f();
        clock_gettime(CLOCK_MONOTONIC, &e);
        sum += nsecs();    
    }
    printf("%lld\n",(sum/total) - overhead);
}
void time_handle(int input){
    if(input == 1){ lets_time(empty); }
}
