#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **string_split(const char *input, const char *sep, int *num_words){
  
    int last_i = 0;
    char** slist = (char**)calloc(1, sizeof(char*));
    char buffer[4000];
    for(int i = 0; i < (int)strlen(input); i++){
        char tmp[2];
        tmp[0] = input[i];
        tmp[1] = '\0';
        
        if(strcspn(sep,tmp) != (int)strlen(sep) || i == (int)strlen(input) - 1){
            if(i != last_i){
        
                if(*num_words !=0){
                    slist = realloc(slist, (1+*num_words) * sizeof(char*));
                } 
               
                // need to fix strn copy size and . error to end the code
                strncpy(buffer,input+(last_i),i-last_i+1);
                buffer[i-last_i+1] = '\0';
                slist[*num_words] = (char*)malloc(strlen(buffer) * sizeof(char));
                strncpy(slist[*num_words],buffer,strlen(buffer));
                *num_words += 1;
                memset(buffer,'\0',sizeof(buffer));
            
            }
            last_i = i;
            
        }
        if(strcspn(sep,tmp) < strlen(sep) && (i == 0 || i == (int)strlen(input) - 1)){
            if(*num_words !=0){
                slist = realloc(slist, (1+*num_words) * sizeof(char*));
            }
            
            slist[*num_words] = (char*)malloc(sizeof(char));
            strncpy(slist[*num_words],"",1);
            *num_words += 1;
            last_i = i+1;
        }
    }
    return slist;
}
