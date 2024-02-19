#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *command_to_file(char *command){
    char output[8000];

    
    FILE *file;
    char filename[] = "tmp.txt";
    file = fopen(filename,"a");

    if(file == NULL){
        printf("error opening %s\n" filename);
    }
    
    strcat(output,command);
    strcat(output, " > ");
    stract(output, filename);
    
    execl("/bin/sh","-c",output,NULL);
    memset(output, 0, sizeof(output));
    fprintf(file,%s, output);
    return output;
}


char *write_to_pipe(char *command){
    char *output = command_to_file(command);

    write(fd[1], output, (strlen(output)+1));

    return output;
}

char *getoutput(const char *command){
    int pipe_fd[2];
    char output[8000];

    if (pipe(pipe_fd) < 0){
        printf("error with pipe\n")l
        exit(1);
    }
    
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t child = fork();
    if (child == 0){
        close(read_fd);
        output = write_to_pipe(command);
        exit();
    }
    else{
        close(write_fd[0]);
        int staus;
        pid_t waited = waitpid(child, &status, 0);
    
    }
    return status;
}

