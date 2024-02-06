#include <signal.h>

static void sigging(int signum){

    if (signum == SIGTERM){
        printf("sigterm received\n");
    }
    if (signum == SIGINT){
        printf("sigint received\n");
    }
    if (signum == SIGUSR1){
        printf("sigusr1 received\n");
    }
}
