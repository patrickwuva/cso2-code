#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_barrier_t barrier; // optional: to hopefully make deadlock more consistent

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void *eat(void *arg) {
    int n = (int) (long)arg;
    int r = 0; // retries
    srand(time(NULL) + n);
    int f = n;
    int s = (n + 1) % 5;

    while(1){
        if (pthread_mutex_trylock(&chopstick[f]) == 0){
            if(pthread_mutex_trylock(&chopstick[s]) == 0){
                
                printf("Philosopher %d got chopstick %d\n", n, f); 
                printf("Philosopher %d got chopstick %d\n", n, n);
                sleep(1);
                
                pthread_mutex_unlock(&chopstick[f]);
                pthread_mutex_unlock(&chopstick[s]);
                break;
            }

            else {
                pthread_mutex_unlock(&chopstick[f]);
            }
        }
        
        int backoff = (50 * (1 << r)) + (rand() % (50 * (1 << r)));
        r++;
        usleep(backoff);
        
        if (r > 5){
            r = 0;
        }
    }
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
