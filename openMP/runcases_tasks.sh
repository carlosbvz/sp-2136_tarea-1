#!/usr/bin/bash 


for i in 1 2 4 8 16 32;do 
    export OMP_NUM_THREADS=$i
    echo $i >> log_tasks.out
    for j in {0..10};do
        ./merge_tasks 100000 0 1000 >> log_tasks.out
    done
done