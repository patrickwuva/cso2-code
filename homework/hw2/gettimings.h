#define _XOPEN_SOURCE 700
#ifndef GETTIMINGS_H
#define GETTIMINGS_H
long long avg_over();
long long nsecs();
long long pid();
long long command();
long long self_sig();
long long send_sig();
int get_pid();
void receive_sig();
void record_timings(char *data); 
#endif
