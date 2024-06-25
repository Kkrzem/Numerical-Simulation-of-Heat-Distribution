// pth-midterm-question.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include "functions.h"
#include "quinn_macros.h"
#include "pth-barrier.h"
#include <string.h>

typedef struct {
    int thread_id;
    int num_threads;
    int N;
    int numIterations;
    double *A;
    double *B;
    double *buffer;
    my_pthread_barrier_t *barrier;
} thread_data_t;

void* thread_work(void* arg) {
    thread_data_t *data = (thread_data_t*) arg;
    int start = BLOCK_LOW(data->thread_id, data->num_threads, data->N);
    int end = BLOCK_HIGH(data->thread_id, data->num_threads, data->N);
    double *A = data->A, *B = data->B, *tmp;

    for (int iter = 0; iter < data->numIterations; ++iter) {
        // Each thread computes its portion of the array, except the boundary elements
        for (int j = start + (data->thread_id == 0 ? 1 : 0); j <= end - (data->thread_id == data->num_threads - 1 ? 1 : 0); ++j) {
            A[j] = (B[j - 1] + B[j] + B[j + 1]) / 3.0;
        }

        my_pthread_barrier_wait(data->barrier);

        if (data->buffer != NULL) {
            // The first and last thread handle copying of the boundary values
            int copyStart = start;
            if (data->thread_id == 0) {
                copyStart = 1; // Skip the very first element
            }
            int copyEnd = end;
            if (data->thread_id == data->num_threads - 1) {
                copyEnd = data->N - 2; // Skip the very last element
            }
            memcpy(data->buffer + iter * data->N + copyStart, A + copyStart, (copyEnd - copyStart + 1) * sizeof(double));
        }

        my_pthread_barrier_wait(data->barrier);
        
        tmp = A;
        A = B;
        B = tmp;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    double startOverall, endOverall, startWork, endWork, startIO2, endIO2;
    double elapsedOverall, elapsedWork, elapsedIO;
    GET_TIME(startOverall);

    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Usage: %s <length> <numIterations> [<out file>] <number of threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);
    int numIterations = atoi(argv[2]);
    char* outFile = NULL;
    int num_threads;

    if (argc == 5) {
        outFile = argv[3];
        num_threads = atoi(argv[4]);
    } else {
        num_threads = atoi(argv[3]);
    }

    double *A = malloc(N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double *buffer = NULL;

    if (outFile != NULL) {
        buffer = malloc(N * numIterations * sizeof(double));
        if (buffer == NULL) {
            fprintf(stderr, "Error allocating memory for the output buffer.\n");
            exit(EXIT_FAILURE);
        }
        // Initialize the buffer with boundary values for all iterations
        for (int i = 0; i < numIterations; ++i) {
            buffer[i * N] = 1.0;
            buffer[i * N + N - 1] = 1.0;
        }
    }

    // Initialize the array with boundary values
    A[0] = 1.0;
    A[N - 1] = 1.0;
    memcpy(B, A, N * sizeof(double));

    my_pthread_barrier_t barrier;
    my_pthread_barrier_init(&barrier, num_threads);

    GET_TIME(startWork);
    pthread_t threads[num_threads];
    thread_data_t thread_args[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        thread_args[i] = (thread_data_t){i, num_threads, N, numIterations, A, B, buffer, &barrier};
        pthread_create(&threads[i], NULL, thread_work, &thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    GET_TIME(endWork);
    elapsedWork = endWork - startWork;

    printf("Debug: About to write to file '%s'\n", outFile ? outFile : "None");

    GET_TIME(startIO2);
    if (outFile != NULL) {
        write_to_file(outFile, buffer, N, numIterations);
    }
    GET_TIME(endIO2);

    my_pthread_barrier_destroy(&barrier);
    free(A);
    free(B);
    free(buffer);

    GET_TIME(endOverall);
    elapsedOverall = endOverall - startOverall;
    elapsedIO = endIO2 - startIO2;

    printf("Parallel Overall Time: %.6f\n", elapsedOverall);
    printf("Parallel Work Time: %.6f\n", elapsedWork);
    printf("Parallel I/O Time: %.6f\n", elapsedIO);

    return 0;
}
