#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **string_split(const char *input, const char *sep, int *num_words){
   
    int last_sep = -1;

    char** slist = (char**)calloc(1, sizeof(char*));
    char buffer[4000];
    for(int i = 0; i < (int)strlen(input); i++){
        char tmp[2];
        tmp[0] = input[i];
        tmp[1] = '\0';
        
        if(strcspn(sep,tmp) != (int)strlen(sep) || i == (int)strlen(input) - 1){
             
            if(i != last_sep + 1 || i != 0) {
       
                *num_words += 1; 
                
                if(*num_words != 1){
                    slist = realloc(slist, (*num_words) * sizeof(char*));
                } 
                
                int size = i-last_sep;

                if(i == (int)strlen(input) - 1 && strcspn(sep,tmp) == strlen(sep)){
                    size += 1;
                }
                strncpy(buffer,input + last_sep+1, size-1);

                buffer[size] = '\0';
                
                slist[*num_words - 1] = (char*)malloc(strlen(buffer) * sizeof(char));
                strncpy(slist[*num_words - 1],buffer,strlen(buffer));
                slist[*num_words - 1][strlen(buffer)] = '\0'; 
                //printf("s: %s in arr: %s len: %zu i: %d sep: %d\n",buffer,slist[*num_words-1],strlen(buffer),i,last_sep);
                memset(buffer,'\0',sizeof(buffer));
            
            }
            last_sep = i;
            
        }
        
        if((i == 0 || i == (int)strlen(input) - 1) && strcspn(sep,tmp) != (int)strlen(sep)){
            *num_words += 1;           
            if(*num_words != 1){
                slist = realloc(slist, (*num_words) * sizeof(char*));
            }
            

            slist[*num_words-1] = (char*)malloc(sizeof(char));
            strncpy(slist[*num_words-1],"",1);
            last_sep = i;
        }

    }
    return slist;
}
