#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "split.c"


int main(int argc, char *argv[]){
    char* sep = (char *)malloc(500 * sizeof(char));
    
    if(argc == 1){
        sep = " \t";
        sep = (char *)realloc(sep,sizeof(char)*strlen(sep));
    }

    else{
        for(int i = 1; i < argc; i++){
            strcat(sep, argv[i]);
        }
        sep =(char *)realloc(sep,sizeof(char)*strlen(sep));
    }
    
    char input[4000];
    while(fgets(input, 4000, stdin) != NULL){ 
        // call the split.c with input;
        if(strlen(input) == 2 && input[0] == '.'){
            return 0;
        }
        memset(input, 0, sizeof(input));
    }
}
