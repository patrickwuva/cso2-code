#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char **string_split(const char *input, const char *sep, int *num_words){
  
    printf("sep: %s size: %zu\n",sep,strlen(sep));
    int last_i = 0;

    char** slist = (char**)calloc(1, sizeof(char*));
    char buffer[4000];
    for(int i = 0; i < (int)strlen(input); i++){
        char tmp[2];
        tmp[0] = input[i];
        tmp[1] = '\0';
        if(strcspn(sep,tmp) != (int)strlen(sep) || i == (int)strlen(input) - 1){
            if(*num_words !=0){
                slist = realloc(slist, (1+*num_words) * sizeof(char*));
            }
   //need to double check how it is proccesing this not doing multiple seps in a row properly 
            strncpy(buffer,input+(last_i),i-last_i);
            buffer[i-last_i] = '\0';
           
            slist[*num_words] = (char*)malloc(strlen(buffer) * sizeof(char));
            strncpy(slist[*num_words],buffer,strlen(buffer));
            *num_words += 1;
            last_i = i+1;
            
            memset(buffer,'\0',sizeof(buffer));
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
