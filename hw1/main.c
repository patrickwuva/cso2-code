#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "split.c"

int size = 0;

void print_result(char **result){

    printf("num words %d\n",size);
}
int main(int argc, char *argv[]){
    char* sep = (char *)malloc(500 * sizeof(char));
    char **result;

    if(argc == 1){
        sep = " \t";// bug here
    }

    else{
        for(int i = 1; i < argc; i++){
            strcat(sep, argv[i]);
        }
        sep =(char *)realloc(sep,sizeof(char)*strlen(sep));
    }
    
    char input[4000];
    while(fgets(input, 4000, stdin) != NULL){ 
        if(strlen(input) == 2 && input[0] == '.'){
            return 0;
        }
        result = string_split(input, sep, &size);
        print_result(result);
        memset(input, 0, sizeof(input));
        size = 0;
    }
}
