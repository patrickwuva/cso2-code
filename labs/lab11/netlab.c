#include <stdio.h>
#include <stdlib.h>
#include "netsim.h"

typedef struct{
    char *data;
    size_t len;
    int got_ack;
    int tid;
} packet;

packet *packets = NULL;

void init_packets(){
    packets = calloc(6, sizeof(packet));
}

int get_csum(char *data){
    int csum = 0;
    for (int i = 1; i < 5; i++){
        csum ^= data[i];
    }
    return csum;
}

void check_ack(void *arg){
    int id = (int)(long)arg;
    printf("pgot? %d\n", packets[id].got_ack);
    if (!packets[id].got_ack){
        send(packets[id].len, packets[id].data);
    }
}

void send_ack(int id){
    char data[5];
    data[1] = 'A'; data[2] = 'C'; data[3] = 'K'; data[4] = id;
    data[0] = get_csum((char*)data);
    packets[id].data = data;
    packets[id].len = 5;
    packets[id].got_ack = 0;
    packets[id].tid = setTimeout(check_ack, 4000, (void*)(long)id);
    send(5, data);
}

void recvd(size_t len, void* _data) {
    char *data = _data;
    fwrite(data+3,1,len-3,stdout);
    fflush(stdout);
    int id = data[1];
    send_ack(id);
    packets[id].got_ack = 1;
    clearTimeout(packets[id].tid);
}

int main(int argc, char *argv[]) {
    // this code should work without modification

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    init_packets();
    
    char data[5];
    data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = argv[1][0];
    data[0] = get_csum((char*)data);
    send(5, data);
    waitForAllTimeoutsAndMessagesThenExit();
}




