#include <stdio.h>
#include <stdlib.h>
#include "netsim.h"

int *seqs = NULL;
int no_seqs = 0;

int get_csum(char *data){
    int csum = 0;
    for (int i = 1; i < 5; i++){
        csum ^= data[i];
    }
    return csum;
}

void ack(void *last){
    char data[5];
    data[1] = 'A'; data[2] = 'C'; data[3] = 'K'; data[4] = *(int *)last;
    data[0] = get_csum((char*)data);
    free(last);
    send(5,data);
}

void recvd(size_t len, void* _data) {
    char *data = _data;
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);
    int c_seq = data[1];
    if(no_seqs == 0){
        no_seqs = data[2];   
        seqs = (int *)calloc(no_seqs -1, sizeof(int));
    }
    if (c_seq == 1){
        for (int i = 1; i < no_seqs - 1; i++){
            int *c_seq_pt = malloc(sizeof(int));
            *c_seq_pt = i;
            seqs[c_seq - 1] = setTimeout(ack, 2500, (void *)c_seq_pt);
        }      
    }
    else{
        clearTimeout(seqs[c_seq - 1]);
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




