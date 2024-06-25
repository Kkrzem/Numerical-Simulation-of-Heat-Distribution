// Author: Karolin Krzeminski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include <omp.h>
#include "timer.h"


int main(int argc, char* argv[]){
    double startOverall, endOverall, startWork, endWork, startIO2, endIO2;
    double elapsedOverall, elapsedWork, elapsedIO;

    GET_TIME(startOverall)

    if (argc < 4 || argc > 5) { 
        printf("usage: %s <length> <numIterations> [<out file>] <number of threads>\n", argv[0]);
        exit(1);
    }
    

    int N = atoi(argv[1]);
    int numIterations = atoi(argv[2]);
    char* outFile = NULL; 
    int thread_count;

    if (argc == 5) {
        outFile = argv[3]; // If outFile is provided, use it
        thread_count = atoi(argv[4]);
        omp_set_num_threads(thread_count);
    } else {
        thread_count = atoi(argv[3]);
        omp_set_num_threads(thread_count);
    }

    
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));
    double *all_data = (double *)malloc(N * numIterations * sizeof(double));
    double *tmp;
    
    // Initial conditions
    memset(A, 0, N * sizeof(double));
    A[0] = 1.0; A[N-1] = 1.0;
    memcpy(B, A, N * sizeof(double));

    GET_TIME(startWork)
    omp_run(A, B, N, numIterations, all_data);
    GET_TIME(endWork)
    
    GET_TIME(startIO2)
    if (outFile != NULL) {
        write_to_file(outFile, all_data, N, numIterations);
    }   
    GET_TIME(endIO2)
    
    free(A);
    free(B);
    free(all_data);
    

    GET_TIME(endOverall)

    elapsedOverall = endOverall-startOverall;
    elapsedWork = endWork - startWork;
    elapsedIO= endIO2-startIO2;

    printf("Parallel Overall Time: %.6f\n", elapsedOverall);
    printf("Parallel Work Time: %.6f\n", elapsedWork);
    printf("Parallel I/O Time: %.6f\n", elapsedIO);
    return 0;


}