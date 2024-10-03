#!/usr/bin/bash 


for i in 1 2 4 8 16 32;do 
    export OMP_NUM_THREADS=$i
    echo $i >> log_sections.out
    for j in {0..10};do
        ./merge_sections 100000 0 1000 >> log_sections.out
    done
done