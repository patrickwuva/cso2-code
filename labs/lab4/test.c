#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t child = fork();

    if(child == 0){
        
    }
    else{
        printf("child %d\n", child);
    }
    printf("done\n");

    return 0;
}
