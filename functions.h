// Author: Karolin Krzeminski

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void print_array(double *A, int N);
void run_iterations(double *A, double *B, int N, int numIterations, double *all_data);
void omp_run(double *A, double *B, int N, int numIterations, double *all_data);
void write_to_file(char *filename, double *A, int N, int numIterations);
#endif