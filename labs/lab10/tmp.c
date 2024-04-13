//map parallel, reduction = fancy reduce
double geomean7(unsigned char *s, size_t n){
    int threads;
    #ifdef OPENMP_ENABLE
        #pragma omp parallel
            #pragma omp master
                threads = omp_get_num_threads();
    #else
        threads = 1;
    #endif
    int per_line = 64/sizeof(double);
    double *results = (double*)malloc(threads * per_line * sizeof(double));
    #pragma omp parallel
    {
        int id = 0;
        #ifdef OPENMP_ENABLE
            id = omp_get_thread_num();
        #else
            id = 0;
        #endif
        double sum = 0;
        #pragma omp for nowait
        for(size_t i = 0; i < n; i+=1){
            if(s[i] > 0){
                sum += log((double)s[i]) / n;
            }
        }
        results[id * per_line] += sum;
    }
    double answer = 0;
    for(int i = 0; i<threads; i+=1){
        answer += results[i * per_line];
    }
    free(results);
    return exp(answer);
}
