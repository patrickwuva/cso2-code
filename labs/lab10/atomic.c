#include <stdio.h> // fopen, fread, fclose, printf, fseek, ftell
#include <math.h> // log, exp
#include <stdlib.h> // free, realloc
#include <time.h> // struct timespec, clock_gettime, CLOCK_REALTIME
#include <errno.h>


// computes the geometric mean of a set of values.
// You should use OpenMP to make faster versions of this.
// Keep the underlying sum-of-logs approach.
double geomean(unsigned char *s, size_t n) {
    double answer = 0;
    for(int i=0; i<n; i+=1) {
        if (s[i] > 0) answer += log(s[i]) / n;
    }
    return exp(answer);
}

// map = even split, reduction = atomic
double geomean1(unsigned char *s, size_t n) {
    double answer = 0;
    #pragma omp parallel for
    for(int i=0; i<n; i+=1) {
        if (s[i] > 0){
            #pragma omp atomic update
            answer += log(s[i]) / n;
        }
    }
    return exp(answer);
}

// map = task queue larger task,dynamic schedule, reduction = atomic
double geomean2(unsigned char *s, size_t n) {
    double answer = 0;
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < n; i+=1){
        if (s[i] > 0){
            #pragma omp atomic update
            answer += log(s[i]) / n;
        }
    }
    return exp(answer);
}

// map = task queue, reduction = atomic capture update
double geomean3(unsigned char *s, size_t n) {
    double answer = 0;
    int j = 0;
    #pragma omp parallel
    while (1) {
        int i;
        #pragma omp atomic capture
        i = j++;
        if (i >= n) break;
        
        if (s[i] > 0){
            #pragma omp atomic update
            answer += log(s[i]) / n;
        }
    }
    return exp(answer);
}

// map = even split, reduction = many to few
double geomean4(unsigned char *s, size_t n) {
    double answer = 0;
    #pragma omp parallel
    {

        int local = 0;
        #pragma omp for nowait
        for(int i = 0; i<n; i+=1){
            if (s[i] > 0){
                answer += log(s[i]) / n;
            }
        }
    }
    return exp(answer);
}

/// nanoseconds that have elapsed since 1970-01-01 00:00:00 UTC
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec*1000000000 + t.tv_nsec;
}


/// reads arguments and invokes geomean; should not require editing
int main(int argc, char *argv[]) {
    double (*geomeans[])(unsigned char*, size_t) = {geomean, geomean1, geomean2, geomean3, geomean4};

    // step 1: get the input array (the bytes in this file)
    char *s = NULL;
    size_t n = 0;
    for(int i=1; i<argc; i+=1) {
        // add argument i's file contents (or string value) to s
        FILE *f = fopen(argv[i], "rb");
        if (f) { // was a file; read it
            fseek(f, 0, SEEK_END); // go to end of file
            size_t size = ftell(f); // find out how many bytes in that was
            fseek(f, 0, SEEK_SET); // go back to beginning
            s = realloc(s, n+size); // make room
            fread(s+n, 1, size, f); // append this file on end of others
            fclose(f);
            n += size; // not new size
        } else { // not a file; treat as a string
            errno = 0; // clear the read error
        }
    }

    
    // step 2: invoke and time the geometric mean function
    int iter = 100;
    for(int i = 0; i < 5; i+=1){
        double answer = 0;
        long long t0 = 0;
        long long t1 = 0;
        for(int j = 0; j < iter; j+=1){
            t0 = nsecs();
            double answer += geomeans[i]((unsigned char*)s, n); 
            t1 = nsecs();
        }
        avg_answer += answer;
        printf("%lld ns to process %zd characters: %g\n", t1-t0, n, answer);
    }
    free(s);
}







