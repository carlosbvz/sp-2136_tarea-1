#!/bin/bash


export LD_LIBRARY_PATH=/home/jkhansell/opencilk/lib/clang/14.0.6/lib/x86_64-unknown-linux-gnu:$LD_LIBRARY_PATH
export PATH=/home/jkhansell/opencilk/include:$PATH
export PATH=/home/jkhansell/opencilk/bin:$PATH

/home/jkhansell/opencilk/bin/clang++ merge_sort.cpp -fcilkplus -v -o merge_sort
