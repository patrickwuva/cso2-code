#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

char *read_from_pipe(int fd){
    char *output = NULL; 
   
    FILE *pipeit = fdopen(fd, "r");
    
    if (pipeit == NULL){
        perror("pipe is dog water");
        exit(EXIT_FAILURE);
    }
    size_t size = 0;
    size_t total = 0;
    ssize_t fbytes;
    char *buff = NULL;
    while ((fbytes = getline(&buff, &size, pipeit)) > -1){
        char *new_output = realloc(output, total + fbytes + 1);
        if(!new_output){
            perror("realloc");
            free(buff);
            fclose(pipeit);
            free(output);
            return NULL;
        } 
        output = new_output;
        memcpy(output + total, buff, fbytes);
        total += fbytes;
        output[total] = '\0';
    }
    free(buff);

    return output;
}

char **update_args(const char **argv_base, int c){
    int size = 1;
    while(argv_base[size] != NULL){
        size++;    
    }
    size+=2;

    char **args = malloc(size * sizeof(char*));
   
    
    for(int i = 0; i < size - 2; i++){
        args[i] = strdup(argv_base[i]);
    }
    char pidstr[20];
    sprintf(pidstr, "%d", c);
    
    args[size-2] = strdup(pidstr);
    args[size-1] = NULL;
    
    return args;
}
char *getoutput(const char *command){
    int pipe_fd[2];
    pipe(pipe_fd);

    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t child = fork();
    if (child == 0){
        close(read_fd);
        dup2(write_fd,1);
        close(write_fd);
        execl("/bin/sh", "sh", "-c", command, NULL);
        exit(EXIT_FAILURE);
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
    int pipe_fd[2];
    pipe(pipe_fd);
    
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid;
    

    for(int i = 0; i < count; i++){
        char **args = update_args(argv_base, i);    
        pid = fork();
        if(pid == 0){
            close(read_fd);
            dup2(write_fd,1);
            execv(argv_base[0],args);
            _exit(0);
        } else {
            free(args);
        }
    }
    while (wait(NULL) > 0);
    close(write_fd);
    char *output = read_from_pipe(read_fd);
    close(read_fd);
    return output;
}




