#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
int my_system(const char *command){
    int fd = open("test.txt", O_WRONLY | O_APPEND | O_CREAT);
    dup2(fd,1);
    pid_t child = fork();
    if (child == 0){
        int test = execl("/bin/sh","sh","-c",command,NULL);        
        printf("test\n");
        //printf("status: %d\n",test);
    }
    
    int status;
    pid_t waited = waitpid(child, &status, 0);

    close(fd);
    return status;
}

int main(){
    my_system("sleep 1; echo hi");
}

