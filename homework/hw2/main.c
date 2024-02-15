#include "gettimings.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

long long overhead;

int main(int argc, char *argv[]){
    for(int i = 0; i < 1000000; i++){
        nsecs();
    }

    overhead = avg_over(1000000);   
    

    if(argc == 2){

        if(atoi(argv[1]) == 1){
            
            time_empty();
        }
    
        else if(atoi(argv[1]) == 2){
            pid();
        }
    
        else if(atoi(argv[1]) == 3){
            command();
        }
    
        else if(atoi(argv[1]) == 4){
            self_sig();
        }
    
        else if(atoi(argv[1]) == 5){
            send_sig();
        }

        else if(atoi(argv[1]) == -1){
            receive_sig();
        }
    }
    
    else{
        printf("incorrect input\n");
    }
    
    return 0;
}
