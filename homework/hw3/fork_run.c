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

    char **args = malloc((size) * sizeof(char*));
   
    
    for(int i = 0; i < size - 2; i++){
        args[i] = malloc(strlen(argv_base[i]) + 1);
        strcpy(args[i], argv_base[i]);
    }
    char pidstr[20];
    sprintf(pidstr, "%d", c);
    
    args[size-2] = malloc(strlen(pidstr) + 1);
    char *temp1 = pidstr;
    char *temp2 = NULL;
   
    strcpy(args[size - 2], pidstr);
    args[size - 1] = NULL;
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
    int read_fd = pipe_fd[0];
    int write_fd = pipe_fd[1];
    pid_t pid;

    for(int i = 0; i < count; i++){
        pid = fork();
        if(pid == 0){
            char **updated_args = update_args(argv_base, i);
            close(read_fd);
            dup2(write_fd,1);
            close(write_fd);
            //char **updated_args = update_args(argv_base);
            printf("test: %s\n", updated_args[0]);
            free(updated_args);
            _exit(0);
        } else {
            close(write_fd);
            char *child_output = read_from_pipe(read_fd);
            printf("tesdt: %s\n", child_output);
            printf("test: %zu", strlen(child_output));
            output = realloc(output, strlen(child_output)+1);
            strcat(output, child_output);
            close(read_fd);
  
        }
    }
    

    return output;
}










