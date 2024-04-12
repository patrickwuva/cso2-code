#include <stdio.h>
#include <stdlib.h>
#include "netsim.h"

char data[5];
char input;
int id, total, first, rec;
int sqn = 1;
void send_ack();
int get_csum(){
    int csum = data[1];
    for (int i = 2; i < 5; i++){
        csum ^= data[i];
    }
    return csum;
}

void send_ack(){
    if (input == '1' || input == '2'){
        data[1] = 'A'; data[2] = 'C'; data[3] = 'K'; data[4] = id;
        data[0] = get_csum();
        send(5, data);
    }
}

void recvd(size_t len, void* _data) {
    char *data = _data;
    
    if(input == '0' || input == '1'){
        fwrite(data+3,1,len-3,stdout);
        id = (int)data[1];
        total=(int)data[2];
        data[4] = sqn;
        fflush(stdout);
        send_ack();
    }
    else if (input == '2') {
        clearTimeout(first);
        id = (int)data[1];
        total = (int)data[2];
        if (sqn == id){
            fwrite(data+3,1,len-3,stdout);
            data[4] = id;
            fflush(stdout);
            sqn++;
            rec = id;
            send_ack();
        }
        else if (id != sqn){
            id = rec;
            send_ack();
        }
    }
}

int main(int argc, char *argv[]) {
    // this code should work without modification

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
        return 1;
    }
    data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = argv[1][0];
    data[0] = get_csum();
    input = data[4];
    
    send(5, data);
    waitForAllTimeoutsAndMessagesThenExit();
    if(input == '2'){
        while(1){
            if (sqn == total + 1){
                clearTimeout(first);
                break;
            }
            first = setTimeout(NULL, 1000, (void *)1UL);
            send_ack();
            waitForAllTimeoutsAndMessagesThenExit();
        }
           clearTimeout(first);
    }
}




