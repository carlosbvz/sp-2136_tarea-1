#!/usr/bin/bash 

module load gcc/7.2.0
module load mpich/3.2.1-gcc-7.2.0
module load cmake/3.17.2

for i in 1 2 4 8 16 32;do 
    echo $i >> log_CILK.out
    for j in {0..1};do
        CILK_NWORKERS=$i ./merge_sort 100000000 $i >> log_CILK.out
    done
done