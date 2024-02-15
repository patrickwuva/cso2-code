#include "gettimings.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

pid_t other_pid = 0;
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

static void sig_handle(int signum){
    if(signum == SIGTERM){
        e = nsecs();
        //printf("sig handler executing \n");
        //fflush(stdout);
    }
    if(signum == SIGUSR1){
        kill(other_pid,SIGUSR1);
        //exit(0);
    }
}

static int get_pid(){
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

void self_sig(){

    struct sigaction sa;
    sum = 0;
    total = 100000;
    sa.sa_handler = sig_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &sa, NULL);
     
    for(int i = 1; i < total; i++){
        s = nsecs();
        kill(getpid(), SIGTERM);
        e = nsecs();
        sum += (e - s) - overhead;
    }
    printf("time: %lld\n", sum/total);
}

void send_sig(){
    struct sigaction sa;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    other_pid = get_pid();
    sum = 0;
    total = 10000;
    sa.sa_handler = sig_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    for(int i = 0; i < total; i++){
        int sig;
        s = nsecs();
        kill(other_pid, SIGUSR1);
        pthread_sigmask(SIG_BLOCK, &set, NULL);
        sigwait(&set, &sig);
        //e = nsecs();
        sum += (e - s) - overhead;
    }
    kill(other_pid, SIGINT);
    printf("time: %lld\n", sum/total);
}

void receive_sig(){
    struct sigaction sa;
    other_pid = get_pid();
    sa.sa_handler = sig_handle;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
     
    while(1);
}

