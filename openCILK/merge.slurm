#!/bin/bash
#SBATCH --job-name=merge_sort_job
#SBATCH --output=result.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

# Load necessary modules
module load gcc/7.2.0
module load mpich/3.2.1-gcc-7.2.0
module load cmake/3.17.2

# Run the Makefile to build the executable
make

# Run the compiled program
./merge_sort 1000 2
