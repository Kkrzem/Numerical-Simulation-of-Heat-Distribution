// Author: Karolin Krzeminski

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//print usage of the program
void printUsage() {
    printf("Usage: ./mydiff <file_A> <file_B>\n");
}

//read a matrix from a binary file
double* readMatrix(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fread(rows, sizeof(int), 1, file);
    fread(cols, sizeof(int), 1, file);

    double* matrix = (double*)malloc((*rows) * (*cols) * sizeof(double));
    if (!matrix) {
        perror("Failed to allocate memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(matrix, sizeof(double), (*rows) * (*cols), file);
    fclose(file);

    return matrix;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }

    int rowsA, colsA, rowsB, colsB;
    double *matrixA, *matrixB;

    matrixA = readMatrix(argv[1], &rowsA, &colsA);
    matrixB = readMatrix(argv[2], &rowsB, &colsB);

    if (rowsA != rowsB || colsA != colsB) {
        printf("Error: Matrices dimensions do not match.\n");
        free(matrixA);
        free(matrixB);
        return 1;
    }

    //Calculate Total Sum of Squared Errors (TSSE)
    double tsse = 0.0;
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsA; ++j) {
            double diff = matrixA[i * colsA + j] - matrixB[i * colsB + j];
            tsse += diff * diff;
        }
    }

    // output the TSSE
    printf("Total Sum of Squared Errors (TSSE): %.6f\n", tsse);

    free(matrixA);
    free(matrixB);

    return tsse == 0.0 ? 0 : 1; //return 0 if matrices are the same, 1 otherwise
}
