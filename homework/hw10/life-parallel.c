#include "life.h"
#include <pthread.h>

typedef struct {
    int start;
    int end;
    int id;
    int nthreads;
    int steps;
    pthread_barrier_t* barrier;
    pthread_mutex_t* mutex;
    LifeBoard *state;
    LifeBoard *next_state;
    
} block;



void* thread_it(void *arg){
    block* b = (block*)arg;
    
    for (int i = 0; i < b->steps; i += 1){
        int start = b->start;
        int end = b->end;

        if(b->id == 0) start = 1;
        if(b->id == b->nthreads - 1) end = b->state->width - 1;
        
        for (int y = 1; y < b->state->height - 1; y += 1) {
            for (int x = start; x < end; x += 1) {
                pthread_mutex_lock(b->mutex);
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(b->state, x + x_offset, y + y_offset))
                            live_in_window += 1;
                
                LB_set(b->next_state, x, y,
                    live_in_window == 3  || (live_in_window == 4 && LB_get(b->state, x, y)));
            pthread_mutex_unlock(b->mutex);
            }
        }
        pthread_barrier_wait(b->barrier);
        
        if (b->id == 0){
            LB_swap(b->state, b->next_state);
        }
        pthread_barrier_wait(b->barrier);
    }
    pthread_exit(NULL);
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    pthread_mutex_t m;
    pthread_mutex_init(&m, NULL);
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, threads);
    pthread_t life_threads[threads];
    block blocks[threads];

    int current = 0;
    int b_width = state->width / threads;
    int remainder = state->width % threads;
    LifeBoard *next_state = LB_new(state->width, state->height);
    for (int i = 0; i < threads; i += 1){
        blocks[i].nthreads = threads;
        blocks[i].next_state = next_state;
        blocks[i].state = state;
        blocks[i].barrier = &barrier;
        blocks[i].id = i;
        blocks[i].start = current;
        blocks[i].steps = steps;
        blocks[i].mutex = &m;
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

    LB_del(next_state);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&m);
} 
