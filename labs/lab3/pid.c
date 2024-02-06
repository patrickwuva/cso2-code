#define _XOPEN_SOURCE 700 // request all POSIX features, even with -std=c11
#include <stdlib.h>       // for EXIT_SUCCESS, NULL, abort
#include <stdio.h>        // for getline
#include <unistd.h>       // for getpid
#include <signal.h>

pid_t other_pid = 0;

static void sigging(int signum){

    if (signum == SIGTERM){
        printf("sigterm received\n");
    }:
    )
    if (signum == SIGINT){
        printf("sigint received\n");
    }
    if (signum == SIGUSR1){
        printf("sigusr1 received\n");
    }
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = sigging;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    printf("This process's ID: %ld\n", (long) getpid());
    char *line = NULL; size_t line_length = 0;
    do {
        printf("Enter other process ID: ");
        if (-1 == getline(&line, &line_length, stdin)) {
            perror("getline");
            abort();
        }
    } while ((other_pid = strtol(line, NULL, 10)) == 0);
    free(line);
    return EXIT_SUCCESS;
}
