#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int size;


void cleanup(char **result){

    for(int i = 0; i< size; i++){
        free(result[i]);
    }
    free(result);
}

void print_result(char **result){
    for(int i = 0; i < size; i++){
        printf("[%s]",result[i]);    
    }
    printf("\n");
    cleanup(result);
}

int main(int argc, char *argv[]){
    char *sep = (char *)calloc(100,sizeof(char));
    char **result = {0};

    if(argc == 1){
        strcpy(sep," \t");
    }

    else{
        for(int i = 1; i < argc; i++){
            strcat(sep, argv[i]);
        }
    }
    
    char input[4000] = {0};
    while(fgets(input, 4000, stdin) != NULL){ 
        if(strlen(input) == 2 && input[0] == '.'){
            free(sep);
            return 0;
        }
        input[strlen(input) -1] = '\0';
        result = string_split(input, sep, &size);
        print_result(result); 
        memset(input, '\0', sizeof(input));
    }
}
