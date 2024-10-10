#!/usr/bin/bash 

for i in 1 2 4 8 16 32;do 
    export CILK_NWORKERS=$i
    echo $i >> log_CILK.out
    for j in {0..1};do
        ./merge_sort 100000000 $i >> log_CILK.out
    done
done