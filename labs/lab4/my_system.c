#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
int my_system(const char *command){
   
    pid_t child = fork();

    if (child == 0){
        int test = execl("/bin/sh","sh","-c",command,NULL);        
        printf("status: %d\n",test);
    }
    
    int status;
    pid_t waited = waitpid(child, &status, 0);

    return status;
}

