#define _XOPEN_SOURCE 700
#ifndef GETTIMINGS_H
#define GETTIMINGS_H
void time_empty();
void pid();
void command();
void self_sig();
void send_sig();
void receive_sig();
long long avg_over();
long long nsecs();
#endif
