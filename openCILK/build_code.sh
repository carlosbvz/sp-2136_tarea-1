#!/bin/bash

module load gcc/7.2.0
module load mpich/3.2.1-gcc-7.2.0
module load cmake/3.17.2
g++ -fcilkplus merge_sort.c -o merge_sort
