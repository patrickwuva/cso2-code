#include "gettimings.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

extern long long s, e;

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

long long avg_over(int t){
    int sum = 0;
    for(int i = 0; i < t; i++){
        sum += get_overhead();
    }
    return sum / t;
}

int get_pid(){
    printf("process pid: %d\n", getpid());
    pid_t p = 0;
    char *line = NULL; size_t line_length = 0;
    do {
        printf("Enter other PID: ");
        if (-1 == getline(&line, &line_length, stdin)) {
            perror("getline");
            abort();
        }
    } while ((p = strtol(line, NULL, 10)) == 0);
    free(line);
    return p; 
}

void record_timings(char *data){

    FILE *file;
    char filename[] = "timings.txt";

    file = fopen(filename, "a");

    if(file == NULL){
        printf("error opening %s\n",filename);
    }
    fprintf(file, "%s", data);
    fclose(file);
}
