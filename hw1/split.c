#include <stdlib.h>
#include <stdio.h>
char **string_split(const char *input, const char *sep, int *num_words){
   
    printf("here");
    char** slist = (char**)malloc(2 * sizeof(char*));
   
    for(int i = 0; i < 2; i++){
        slist[i] = (char *)malloc(2 * sizeof(char));
        slist[i] = " ";
    }


    if(*num_words != 0){
        
    }
    return slist;
}
