#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

    printf("parent pid %d\n",getpid());

    pid_t pid;
    for(int i = 0; i < 4; i++){
    
        pid = fork();
        
        if(pid == -1){
            perror("fork failed");
        }

        else if(pid == 0){
            printf("Child pid %d\n",getpid());
            sleep(2);
            _exit(0);
        }
        printf("pid outside loop %d\n", getpid());
    }
}

