// Author: Karolin Krzeminski

#include <stdio.h>
#include <stdlib.h>

void print_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%1.2lf ", matrix[i * cols + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary file name>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    int N, numIterations;
    // Read the metadata: number of columns and number of rows (iterations)
    if (fread(&N, sizeof(int), 1, fp) + fread(&numIterations, sizeof(int), 1, fp) != 2) {
        fprintf(stderr, "Failed to read the metadata from the file.\n");
        fclose(fp);
        return 1;
    }

    // Allocate memory for the matrix
    double *matrix = (double *)malloc(N * numIterations * sizeof(double));
    if (matrix == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    // Read the entire matrix data
    if (fread(matrix, sizeof(double), N * numIterations, fp) != (size_t)(N * numIterations)) {
        fprintf(stderr, "Failed to read the matrix data from the file.\n");
        free(matrix);
        fclose(fp);
        return 1;
    }

    // Print the matrix
    print_matrix(matrix, numIterations, N);

    // Clean up
    free(matrix);
    fclose(fp);

    return 0;
}
