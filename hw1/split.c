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
        
        //printf("char: %s strcspn: %zu\n",tmp,strcspn(sep,tmp));

        if(strcspn(sep,tmp) != (int)strlen(sep) || i == (int)strlen(input) - 1){
            if(*num_words !=0){
                slist = realloc(slist, (1+*num_words) * sizeof(char*));
            } 
             
            strncpy(buffer,input+(last_i),i-last_i);
            buffer[i-last_i] = '\0';
           
            slist[*num_words] = (char*)malloc(strlen(buffer) * sizeof(char));
            strncpy(slist[*num_words],buffer,strlen(buffer));
            *num_words += 1;
            memset(buffer,'\0',sizeof(buffer));
            }
            last_i = i+1;
            
        }
        else if(strcspn(sep,tmp) < strlen(sep)-1 && (i == 0 || i == (int)strlen(input) - 1)){
            
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
