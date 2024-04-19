#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barrier; // optional: to hopefully make deadlock more consistent

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void *eat(void *arg) {
    int n = (int) (long)arg;

    int f = n; //first chopstick
    int s = (n+1) % 5;

    if (f > s){
        int t = f;
        f = s;
        s = t;
    }

    // optional: sync up threads to make deadlock hopefully happen more consistently
    pthread_barrier_wait(&barrier);

    // take two chopsticks
    pthread_mutex_lock(&chopstick[f]);
    printf("Philosopher %d got chopstick %d\n", n, f);
    pthread_mutex_lock(&chopstick[s]);
    printf("Philosopher %d got chopstick %d\n", n, s);
    
    printf ("Philosopher %d is eating\n",n);
    sleep(1);
    
    // set them back down
    printf("Philosopher %d set down chopstick %d\n", n, s);
    pthread_mutex_unlock(&chopstick[s]);
    printf("Philosopher %d set down chopstick %d\n", n, f);
    pthread_mutex_unlock(&chopstick[f]);
    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}