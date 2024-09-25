#!/bin/bash
#BATCH --job-name=merge
#SBATCH --output=result.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

./merge_sort 