#define _XOPEN_SOURCE 600 // request all POSIX features, even with -std=c11
#include <stdlib.h>       // for EXIT_SUCCESS, NULL, abort
#include <stdio.h>        // for getline
#include <unistd.h>       // for getpid
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

char *inbox_data;
char *outbox_data;
char inbox_filename[512];
char outbox_filename[512];
int size = 4096;
pid_t other_pid = 0;
static void cleanup(){
    shm_unlink(inbox_filename);
    shm_unlink(outbox_filename);
    munmap(inbox_data, size);
    munmap(outbox_data, size);
}
static void sigging(int signum){

    char msg_int[] = "sigusr1 reciered \n";
    if (signum == SIGTERM){
        cleanup();
        _exit(EXIT_SUCCESS);
    }
    if (signum == SIGINT){
        cleanup();
        kill(other_pid,SIGTERM);
        _exit(EXIT_SUCCESS);
    }
    if (signum == SIGUSR1){
        fputs(inbox_data,stdout);
        fflush(stdout);
        memset(inbox_data, 0, size);
        inbox_data[0] = '\0';
        
    }
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = sigging;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    printf("current pid: %ld\n",(long)getpid());

    char *line = NULL; size_t line_length = 0;
    do {
        printf("Enter other process ID: ");
        if (-1 == getline(&line, &line_length, stdin)) {
            perror("getline");
            abort();
        }
    } while ((other_pid = strtol(line, NULL, 10)) == 0);
    free(line);

    snprintf(inbox_filename, sizeof inbox_filename, "/%d-chat",getpid());
    snprintf(outbox_filename, sizeof outbox_filename, "/%d-chat",other_pid);

    int inbox_fd = shm_open(inbox_filename, O_CREAT | O_RDWR, 0666);
    if(inbox_fd < 0){ printf("error with inbox_fd\n"); }

    int outbox_fd = shm_open(outbox_filename, O_CREAT | O_RDWR, 0666);
    if(outbox_fd <0) { printf("error with outbox_fd\n"); }
    
    ftruncate(inbox_fd, size);
    ftruncate(outbox_fd, size);

    inbox_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, inbox_fd, 0);
    outbox_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, outbox_fd, 0);

    close(inbox_fd);
    close(outbox_fd);

    if (inbox_data == (char*) MAP_FAILED){
        printf("inbox map failed. error\n");
    }
    
    if (outbox_data == (char*) MAP_FAILED){
        printf("outbox map failed. error\n");
    }
    
    char buffer[4096];
    while(1){
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            kill(other_pid, SIGTERM);
            break;
        }
        memcpy(outbox_data, buffer, strlen(buffer));
        kill(other_pid, SIGUSR1);
        while(outbox_data[0]) { usleep(10000); }
    }

    cleanup();
    return EXIT_SUCCESS;
}
