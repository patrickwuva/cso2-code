#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


long file_size(char *filename){
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fclose(file);
    return len;
}

char *read_from_pipe(int fd){
    ssize_t bytes;
    char buffer[1024];
    char filename[] = "tmp.txt";

    FILE *file = fopen(filename, "w+");

    ssize_t fbytes;
    
    while((fbytes = read(fd,buffer,sizeof(buffer) - 1)) > 0){
        fwrite(buffer, 1, fbytes, file);
    }
    
    fflush(file);
    rewind(file);

    long len = file_size(filename);

    char *output = (char*)malloc(len + 1);
        
    fread(output, 1, sizeof(output), file);
    fclose(file);
    remove(filename);
    return output;
}

char *getoutput(const char *command){
    fflush(stdout);
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0){
        printf("error with pipe\n");
        exit(1);
    }
    
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t child = fork();
    
    if (child == 0){
        dup2(write_fd,1);
        close(write_fd);
        close(read_fd);
        execl("/bin/sh", "sh", "-c", command, NULL);
        _exit(0);
    }
    
    else{
        close(write_fd);   
        char *output = read_from_pipe(read_fd);
        close(read_fd);
        waitpid(child, NULL, 0);
        return output;
    }
    return NULL;
}

char *parallelgetoutput(int count, const char **argv_base){
    pid_t pids[count];

    for(int i = 0; i < count; i++){
        pids[i] = fork();
        
        if(pids[i] == 0){
            

        }
        
    }

    return NULL;
}










