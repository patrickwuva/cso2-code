#include "gettimings.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    
    if(argc == 2){

        if(atoi(argv[1]) == 1){
            time_handle(1);
        }
    
        else if(atoi(argv[1]) == 2){
            time_handle(2);
        }
    
        else if(atoi(argv[1]) == 3){
            time_handle(3);
        }
    
        else if(atoi(argv[1]) == 4){
            time_handle(4);
        }
    
        else if(atoi(argv[1]) == 5){
            time_handle(5);
        }

        else if(atoi(argv[1]) == -1){
            time_handle(-1);
        }
    }
    
    else{
        printf("incorrect input\n");
    }
    
    return 0;
}
