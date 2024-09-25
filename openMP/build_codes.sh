#!/bin/bash

g++ -o merge_serial mergesort_serial.cpp
g++ -fopenmp -o merge_sections mergesort_OpenMP_sections.cpp 
g++ -fopenmp -o merge_tasks mergesort_OpenMP_tasks.cpp 
