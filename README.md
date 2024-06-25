# Numerical-Simulation-of-Heat-Distribution
Developed a numerical simulation program to model temperature distribution across a metal strip, comparing serial and parallel implementations.
Components

functions.c
Contains core functions used across multiple applications:

print_array(): Prints an array of doubles to the standard output.
run_iterations(): Runs a specified number of iterations, updating an array according to a predefined rule.
omp_run(): Similar to run_iterations but parallelized using OpenMP.
write_to_file(): Writes array data to a file.

midterm-question.c
A C program that demonstrates serial execution of the iterative process defined in functions.c. It includes timing for overall execution, computation, and I/O operations.

omp-midterm-question.c
An extension of midterm-question.c that incorporates OpenMP to parallelize the iterative process, along with command-line options to specify the number of threads.

pth-midterm-question.c
An extension of midterm-question.c that incorporates Pthreads to parallelize the iterative process, along with command-line options to specify the number of threads.

mydiff.c
Compares two binary files containing matrices and reports the Total Sum of Squared Errors (TSSE) between them.

display_image.py
A Python script that reads binary data representing a matrix, plots the data as a contour map, and saves it to an image file.

Compilation
Prerequisites
GCC compiler for C programs.
Python 3.x with NumPy and Matplotlib libraries for running the Python script.
OpenMP support in the GCC compiler for parallel execution.

Compilation Commands

gcc -Wall -o midterm-question midterm-question.c functions.c -lm
gcc -Wall -o omp-midterm-question omp-midterm-question.c functions.c -lm -fopenmp
gcc -Wall -o mydiff mydiff.c -lm

Running the Programs

./midterm-question <length> <numIterations> [<out file>]
./omp-midterm-question <length> <numIterations> [<out file>] <number of threads>
./pth-midterm-question <N> <iterations> <number of threads> <output file (optional)>
./mydiff <file_A> <file_B>

Run the Python script to visualize data:
python display_image.py <input file name>
