#include <stdio.h>
#include "netsim.h"

int get_csum(char *data){
    int csum = 0;
    for (int i = 1; i < 5; i++){
        csum ^= data[i];
    }
    return csum;
}

void ack(int last){
    char data[5];
    data[1] = 'A'; data[2] = 'C'; data[3] = 'K'; data[4] = last;
    data[0] = get_csum((char*)data);
    send(5,data);
}

void recvd(size_t len, void* _data) {
    char *data = _data;
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);
    printf("[");
    for(int i = 0; i < len; i++){
        if (i < 3) printf("%d,",data[i]);
    }
    printf("]\n");
    int c_seq = data[1];
    int l_seq = data[2];
    if(c_seq < l_seq){
        ack(c_seq);
    }
}

int main(int argc, char *argv[]) {
    // this code should work without modification
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    char data[5];
    data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = argv[1][0];
    
    data[0] = get_csum((char*)data);
    send(5, data);
     
    waitForAllTimeoutsAndMessagesThenExit();
}
