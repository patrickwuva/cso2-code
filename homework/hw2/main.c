#include "gettimings.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

long long overhead;
int main(int argc, char *argv[]){
    long long result;
    char avg_str[20];
    char data_str[100];
    for(int i = 0; i < 1000000; i++){
        nsecs();
    }

    overhead = avg_over(1000000);   
    
    if(argc == 2){

        if(atoi(argv[1]) == 1){
            result = overhead; //the avg_overhead function is an empty function
            printf("empty avg: %lld nano secs\n",overhead);
            strcat(data_str, "Empty avg: ");
        }
    
        else if(atoi(argv[1]) == 2){
            result = pid();
            strcat(data_str, "Pid avg: ");
        }
    
        else if(atoi(argv[1]) == 3){
            result = command();
            strcat(data_str, "System avg: ");
        }
    
        else if(atoi(argv[1]) == 4){
            result = self_sig();
            strcat(data_str, "Self sig avg: ");
        }
    
        else if(atoi(argv[1]) == 5){
            result = send_sig();
            strcat(data_str, "Send sig avg: ");
        }

        if(atoi(argv[1]) == -1){
            receive_sig();
        }
        else{

        sprintf(avg_str, "%lld", result);
        strcat(data_str, avg_str);
        strcat(data_str, " nano secs \n");
        record_timings(data_str);
    
        }
    }
    else{
        printf("incorrect input\n");
    }
    
    return 0;
}
