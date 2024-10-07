#!/usr/bin/bash 

for i in 1 2 4 8 16 32;do 
    echo $i >> log_CILK.out
    for j in {0..5};do
        ./merge_sort 100000000 $i >> log_CILK.out
    done
done