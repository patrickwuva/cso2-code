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
            char *path = "/bin/echo";
            char pid_str[32];
            sprintf(pid_str, "%d", getpid());
            char *args[] = {"echo","child pid:",pid_str,NULL}; 
            execv("/bin/echo",args);
        }
        printf("pid outside loop %d\n", getpid());
    }
}

