# Define compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall 
# PTHREAD_FLAGS=-lm -lpthread
OMP_FLAGS= -fopenmp
MATH_FLAGS=-lm

# Define the source files and the executable names
SRCS_FUNCTIONS=functions.c
SRCS_MIDTERM_QUESTION=midterm-question.c $(SRCS_FUNCTIONS)
SRCS_OMP_MIDTERM_QUESTION=omp-midterm-question.c $(SRCS_FUNCTIONS)
SRCS_MYDIFF=mydiff.c


MAIN_MIDTERM_QUESTION=midterm-question
MAIN_OMP_MIDTERM_QUESTION=omp-midterm-question
MAIN_MYDIFF=mydiff

# Define all targets
all: $(MAIN_MIDTERM_QUESTION) $(MAIN_OMP_MIDTERM_QUESTION) $(MAIN_MYDIFF)

# Rules for SERIAL
$(MAIN_MIDTERM_QUESTION): 
	$(CC) $(CFLAGS) $(SRCS_MIDTERM_QUESTION) -o $(MAIN_MIDTERM_QUESTION) $(MATH_FLAGS)

$(MAIN_OMP_MIDTERM_QUESTION):
	$(CC) $(CFLAGS) $(OMP_FLAGS) $(SRCS_OMP_MIDTERM_QUESTION) -o $(MAIN_OMP_MIDTERM_QUESTION) $(MATH_FLAGS)

# Rules for mydiff
$(MAIN_MYDIFF):
	$(CC) $(CFLAGS) $(SRCS_MYDIFF) -o $(MAIN_MYDIFF) $(MATH_FLAGS)


# Clean old build files
clean:
	$(RM) $(MAIN_MIDTERM_QUESTION) $(MAIN_OMP_MIDTERM_QUESTION) $(MAIN_MYDIFF) *.o *~



