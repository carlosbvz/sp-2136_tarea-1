#!/bin/bash

g++ -O3 -o merge_serial mergesort_serial.cpp
g++ -fopenmp -O3 -o merge_tasks mergesort_OpenMP_tasks.cpp 
