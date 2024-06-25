// Author: Karolin Krzeminski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "functions.h"


int main(int argc, char* argv[]) {
    double startOverall, endOverall, startWork, endWork, startIO2, endIO2;
    double elapsedOverall, elapsedWork, elapsedIO;
    GET_TIME(startOverall)
    if (argc < 3 || argc > 4) { // Allow for 4 or 5 arguments
        printf("usage: %s <length> <numIterations> [<out file>]\n", argv[0]);
        exit(1);
    }
    int N = atoi(argv[1]);
    int numIterations = atoi(argv[2]);
    char* outFile = NULL;

    if (argc == 4) {
        outFile = argv[3]; // If outFile is provided, use it
    }
    
    double *A = (double *)malloc(N * sizeof(double));
    double *B = (double *)malloc(N * sizeof(double));
    size_t size = (size_t)N * (size_t)numIterations * sizeof(size_t);
    double *all_data = (double *)malloc(size);
    
    
    // Initial conditions
    memset(A, 0, N * sizeof(double));
    A[0] = 1.0; A[N-1] = 1.0;
    memcpy(B, A, N * sizeof(double));
    

    GET_TIME(startWork)
    run_iterations(A, B, N, numIterations, all_data);
    GET_TIME(endWork)

    GET_TIME(startIO2)
    if (outFile != NULL) {
        write_to_file(outFile, all_data, N, numIterations);
    }
    GET_TIME(endIO2)
    
    // Clean up
    free(A);
    free(B);
    free(all_data);
    GET_TIME(endOverall)

    elapsedOverall = endOverall-startOverall;
    elapsedWork = endWork - startWork;
    elapsedIO= endIO2-startIO2;

    printf("Serial Overall Time: %.6f\n", elapsedOverall);
    printf("Serial Work Time: %.6f\n", elapsedWork);
    printf("Serial I/O Time: %.6f\n", elapsedIO);
    
    return 0;
}


