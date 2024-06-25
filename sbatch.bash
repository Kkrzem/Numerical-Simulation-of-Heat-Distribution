#!/bin/bash 

#SBATCH --job-name="omp-midterm-question" 

#SBATCH --output="omp-midterm-question.%j.%N.out" 

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
BASE_I=530

for N in 500000 1000000 1500000 2000000 2500000 3000000 3500000 4000000 
do
    # Calculate I for the current N
    I=$(( (BASE_N * BASE_I) / N ))

    echo "Calculated I for N=$N is $I"

    echo "Running serial program on size $N with $I iterations"
    serial_output=$(./midterm-question $N $I)
    serial_ot=$(echo "$serial_output" | grep "Serial Overall Time:" | awk '{print $4}')
    serial_wt=$(echo "$serial_output" | grep "Serial Work Time:" | awk '{print $4}')
    serial_io=$(echo "$serial_output" | grep "Serial I/O Time:" | awk '{print $4}')

    echo "Serial - N=$N, Overall Time: $serial_ot, Work Time: $serial_wt, I/O Time: $serial_io"
    echo "---------------------------------------"

    for (( P=1; P<=64; P*=2 ))
    do
        # Run parallel program and capture timing information
        echo "Running parallel program on size $N with $I iterations and $P threads"
        parallel_output=$(./pth-midterm-question $N $I $P)
        parallel_ot=$(echo "$parallel_output" | grep "Parallel Overall Time:" | awk '{print $4}')
        parallel_wt=$(echo "$parallel_output" | grep "Parallel Work Time:" | awk '{print $4}')
        parallel_io=$(echo "$parallel_output" | grep "Parallel I/O Time:" | awk '{print $4}')

        # Calculate speedup for OT WT IO
        speedup_ot=$(echo "scale=6; $serial_ot / $parallel_ot" | bc)
        speedup_wt=$(echo "scale=6; $serial_wt / $parallel_wt" | bc)
        speedup_io=$(echo "scale=6; $serial_io / $parallel_io" | bc)
        
        # Calculate efficiency for OT WT IO
        efficiency_ot=$(echo "scale=6; $speedup_ot / $P" | bc)
        efficiency_wt=$(echo "scale=6; $speedup_wt / $P" | bc)
        efficiency_io=$(echo "scale=6; $speedup_io / $P" | bc)
        
        printf "Parallel - Overall Time: %.6f, Work Time: %.6f, I/O Time: %.6f, Speedup OT: %.6f, Speedup WT: %.6f, Speedup I/O: %.6f, Efficiency OT: %.6f, Efficiency WT: %.6f, Efficiency IO: %.6f\n" $parallel_ot $parallel_wt $parallel_io $speedup_ot $speedup_wt $speedup_io $efficiency_ot $efficiency_wt $efficiency_io
    done

    echo "-------------------------------------------------"
done
