#include "gettimings.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern long long overhead;
long long sum;
long long s, e;
long long total = 1000000;
long long nsecs(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}

__attribute__((noinline)) long long get_overhead(){
    s = nsecs();
    e = nsecs();
    __asm__("");
    return e - s;
}

__attribute__((noinline)) int empty(){
    __asm__("");
    return 0;
}

long long avg_over(int t){
    sum = 0;
    for(int i = 0; i < t; i++){
        sum += get_overhead();
    }
    return sum / t;
}

void time_empty(){
    sum = 0;
    for(int i = 0; i < total; i++){
        s = nsecs();
        empty();
        e = nsecs();
        sum += (e-s) - overhead;
    }
    printf("time: %lld\n", sum/total);
}

void pid(){
    for(int i = 0; i < total; i++){
        s = nsecs();
        getppid();
        e = nsecs();
        sum += (e - s) - overhead;
    }
    printf("time: %lld\n", sum/total);
}

void command(){
    sum = 0;
    total = 100;
    for(int i = 0; i < total; i++){
        s = nsecs();
        system("/bin/true");
        e = nsecs();
        sum += (e - s) - overhead;
    }
    printf("time: %lld\n", sum/total);
}




