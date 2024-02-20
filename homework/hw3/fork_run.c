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

char **update_args(const char **argv_base){
    int size = 0;
    while(argv_base[size] != NULL){
        size++;    
    }
   
    char **args = malloc(size * sizeof(char*));
    for(int i = 0; i < size; i++){
        args[i] = strdup(argv_base[i]);
    }

    char pidstr[15];
    sprintf(pidstr, "%d", getpid());
    args[size-2] = pidstr;
    args[size-1] = NULL;

    return args;
}
char *getoutput(const char *command){
    fflush(stdout);
    int pipe_fd[2];
    
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
    fflush(stdout);
    int pipe_fd[2];
    char *output = (char*)malloc(1);
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid;

    for(int i = 0; i < count; i++){
        pid = fork();
        if(pid == 0){
            dup2(write_fd,1);
            close(write_fd);
            close(read_fd);
            char **updated_args = update_args(argv_base);
            execv(argv_base[0], updated_args);
            free(updated_args);
            _exit(0);
        }
    }
    pid_t child;
    while ((child = wait(NULL)) > 0){
        close(write_fd);
        char *child_output = read_from_pipe(read_fd);
        output = realloc(output, strlen(child_output)+1);
        strcat(output, child_output);
        close(read_fd);
        free(output);
        
    }
    return output;
}










