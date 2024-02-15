#include "gettimings.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


// just stop your crying it's a sign of the times

extern long long overhead;
long long sum;
long long s, e;
long long total = 1000000;
pid_t other_pid = 0;

static void sig_handle(int signum){
    if(signum == SIGTERM){
        e = nsecs();
        //kill(other_pid,SIGINT);
    }
    
    if(signum == SIGUSR1){
        kill(other_pid,SIGINT);
    }
}

long long pid(){
    for(int i = 0; i < total; i++){
        s = nsecs();
        getppid();
        e = nsecs();
        sum += (e - s) - overhead;
    }
    printf("time: %lld nsecs\n", sum/total);
    return sum/total;
}

long long command(){
    sum = 0;
    total = 100;
    for(int i = 0; i < total; i++){
        s = nsecs();
        system("/bin/true");
        e = nsecs();
        sum += (e - s) - overhead;
    }
    printf("time: %lld nsecs\n", sum/total);
    return sum/total;
}

long long self_sig(){

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
    printf("time: %lld nsecs\n", sum/total);
    return sum/total;
}

long long send_sig(){
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    
    other_pid = get_pid();
    sum = 0;
    total = 100000;
    
    for(int i = 0; i < total; i++){
        int sig;
        s = nsecs();
        kill(other_pid, SIGUSR1);
        pthread_sigmask(SIG_BLOCK, &set, NULL);
        sigwait(&set, &sig);
        e = nsecs();
        sum += (e - s) - overhead;
    }
    
    kill(other_pid, SIGINT);
    printf("time: %lld nsecs\n", sum/total);
    return sum/total;
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

