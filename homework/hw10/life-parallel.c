#include "life.h"
#include <pthread.h>

typedef struct {
    int start;
    int end;
    int id;
} block;

pthread_barrier_t barrier;

int generations;
LifeBoard *s = NULL;
LifeBoard *next_state = NULL;

void* thread_it(void *arg){
    block* b = (block*)arg;
    
    for (int i = 0; i < generations; i += 1){
        for (int y = 1; y < s->height - 1; y += 1) {
            for (int x = b->start + 1; x < b->end - 1; x += 1) {
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(s, x + x_offset, y + y_offset))
                            live_in_window += 1;
                
                LB_set(next_state, x, y,
                    live_in_window == 3  || (live_in_window == 4 && LB_get(s, x, y)));
            }
        }
        pthread_barrier_wait(&barrier);
        
        if (b->id == 0){
            LB_swap(s, next_state);
        }
        pthread_barrier_wait(&barrier);
    }
    if (b->id == 0){
        LB_del(next_state);
    }
    pthread_barrier_wait(&barrier);
    pthread_exit(NULL);
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    s = state;
    generations = steps;
    pthread_barrier_init(&barrier, NULL, threads);
    pthread_t life_threads[threads];
    block blocks[threads];

    int current = 0;
    int b_width = state->width / threads;
    int remainder = state->width % threads;
    next_state = LB_new(state->width, state->height);
    for (int i = 0; i < threads; i += 1){
        blocks[i].id = i;
        blocks[i].start = current;
        current += b_width;
        if (i < remainder){
            current += 1;
        }
        blocks[i].end = current;
        pthread_create(&life_threads[i], NULL, thread_it, (void*)&blocks[i]);
    }
    
    for (int i = 0; i < threads; i+=1){
        pthread_join(life_threads[i], NULL); 
    }

    pthread_barrier_destroy(&barrier);

} 
