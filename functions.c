// Author: Karolin Krzeminski

#include <stdio.h>
#include <omp.h>
#include <string.h>

void print_array(double *A, int N) {
    for(int i = 0; i < N; i++) {
        printf("%1.2lf ", A[i]);
    }
    printf("\n");
}

void run_iterations(double *A, double *B, int N, int numIterations, double *all_data) {
    double *tmp;
    for(int i = 0; i < numIterations; i++) {
        for(int j = 1; j < N-1; j++) {
            A[j] = (B[j-1] + B[j] + B[j+1]) / 3.0;
        }
        memcpy(all_data + i * N, A, N * sizeof(double));
        // print_array(A, N); // Print the current state
        tmp = A;
        A = B;
        B = tmp;
    }
}

void omp_run(double *A, double *B, int N, int numIterations, double *all_data) {
    double *tmp;
    for(int i = 0; i < numIterations; i++) {
        // Parallelize this loop with OpenMP
        #pragma omp parallel for
        for(int j = 1; j < N - 1; j++) {
            A[j] = (B[j-1] + B[j] + B[j+1]) / 3.0;
        }
        // Ensure all threads are done computing before proceeding
        #pragma omp barrier

        // Only the master thread should execute the print and swap operations
        #pragma omp master
        {
            memcpy(all_data + i * N, A, N * sizeof(double));
            // print_array(A, N);
            tmp = A;
            A = B;
            B = tmp;
        
        }
        // Ensure the master has finished executing the above block
        #pragma omp barrier
    }
}

void write_to_file(char *filename, double *data, int N, int numIterations) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    // Write metadata
    fwrite(&N, sizeof(int), 1, fp);
    fwrite(&numIterations, sizeof(int), 1, fp);
    
    // Write the entire 2D array data
    fwrite(data, sizeof(double), N * numIterations, fp);
    
    fclose(fp);
}