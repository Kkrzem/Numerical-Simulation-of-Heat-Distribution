#!/bin/bash 

#SBATCH --job-name="data-validatory" 

#SBATCH --output="data-validatory.%j.%N.out" 

#SBATCH --partition=shared

#SBATCH --nodes=1 

#SBATCH --ntasks-per-node=1 

#SBATCH --cpus-per-task=64

#SBATCH --account=ccu108 

#SBATCH --export=ALL 

#SBATCH -t 00:45:00

module purge 
module load cpu
module load slurm
module load gcc

# Base values for N=4000000
BASE_N=4000000
BASE_I=80

for N in 500000 1000000 1500000 2000000 2500000 3000000 3500000 4000000 
do
    # Calculate I for the current N
    I=$(( (BASE_N * BASE_I) / N ))

    echo "Calculated I for N=$N is $I"

    # Run serial program and generate output file
    echo "Running serial program on size $N with $I iterations"
    ./midterm-question $N $I "A_$N_serial"

    for (( P=1; P<=64; P*=2 ))
    do
        # Run parallel program and generate output file
        echo "Running parallel program on size $N with $I iterations and $P threads"
        ./omp-midterm-question $N $I "B_$N_$P" $P
        
        # Call mydiff to compare serial and parallel output files
        echo "Comparing output files for N=$N, I=$I, P=$P"
        mydiff_output=$(./mydiff "A_$N_serial" "B_$N_$P")
        echo "$mydiff_output"
    done
done
