#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

char *read_from_pipe(int fd){
    char *output = NULL; 
   
    FILE *pipeit = fdopen(fd, "r");
    size_t size = 0;
    size_t total = 0;
    ssize_t fbytes;
    char *buff = NULL;
    while ((fbytes = getline(&buff, &size, pipeit)) != -1){
        output = realloc(output, size + fbytes + 1);
        if(!output){
            perror("realloc");
            free(buff);
            fclose(pipeit);
            return NULL;
        } 
        memcpy(output + total, buff, fbytes);
        total += fbytes;
        output[total] = '\0';
    }
    free(buff);
    fclose(pipeit);

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
    
    for(int i = 0; i<size; i++){
        printf("%s",args[i]);
    }
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
    
    char *output = (char*)malloc(1);
    output[0] = '\0';

    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid;
    

    for(int i = 0; i < count; i++){
        char **args = update_args(argv_base, i);    
        pid = fork();
        if(pid == 0){
            close(read_fd);
            dup2(write_fd,1);
            close(write_fd);
            execv(argv_base[0],args);
            _exit(0);
        } else {
            free(args);
            close(write_fd);
            char *child_output = read_from_pipe(read_fd);
            output = realloc(output, strlen(output)+strlen(child_output)+1);
            strcat(output, child_output);
            output[strlen(output)] = '\0';
            free(child_output);
            close(read_fd);
        }
    }
    while (wait(NULL) > 0);
    return output;
}










