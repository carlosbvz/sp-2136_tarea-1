#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm> // For std::sort
#include <random>

#define THRESHOLD 16384 //handle small arrays 

void generateRandomArray(int *arr, int size, int minValue, int maxValue)
{
    std::random_device rd;                                     // Obtain a random number from hardware
    std::mt19937 eng(rd());                                    // Seed the generator
    std::uniform_int_distribution<> distr(minValue, maxValue); // Define the range

    for (int i = 0; i < size; i++)
    {
        arr[i] = distr(eng); // Generate random numbers and fill the array
    }
}

void merge(int *arr, int lo, int mid, int hi)
{
    int n1 = mid - lo + 1;
    int n2 = hi - mid;

    int *L = new int[n1];
    int *R = new int[n2];


    #pragma omp taskgroup
    {
        #pragma omp task
        {
            #pragma omp parallel for 
            for (int i = 0; i < n1; i++)
                L[i] = arr[lo + i];
        }
        #pragma omp task
        {
            #pragma omp parallel for
            for (int i = 0; i < n2; i++)
                R[i] = arr[mid + 1 + i];
        }
    }

    //for (int i = 0; i < n1; i++)
    //    L[i] = arr[lo + i];
    //for (int i = 0; i < n2; i++)
    //    R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = lo;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
    
    delete[] L;
    delete[] R;
}

void mergesort(int *A, int lo, int hi)
{
   if (lo < hi)
   {
        if (hi - lo < THRESHOLD)
        {
            std::sort(A + lo, A + hi + 1); // Perform serial sort (std::sort)
        }
        else
        {
            int mid = lo + (hi - lo) / 2;
            
            #pragma omp taskgroup
            {
                #pragma omp task shared(A)
                mergesort(A, lo, mid);
                #pragma omp task shared(A)
                mergesort(A, mid + 1, hi);
            }
            merge(A, lo, mid, hi);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <size> <minValue> <maxValue>" << std::endl;
        return 1; // Exit with error code
    }

    int N = std::atoi(argv[1]);        // Size of the array
    int minValue = std::atoi(argv[2]); // Minimum value for random numbers
    int maxValue = std::atoi(argv[3]); // Maximum value for random numbers
    int *arr = new int[N];
    generateRandomArray(arr, N, minValue, maxValue);

    // Print the unsorted array
    //std::cout << "Unsorted array: ";
    //for (int i = 0; i < N; i++)
    //    std::cout << arr[i] << " ";
    //std::cout << std::endl;

    // Perform the mergesort
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    #pragma omp single
    mergesort(arr, 0, N - 1);

    auto end = std::chrono::high_resolution_clock::now();

    // Print the sorted array
    //std::cout << "Sorted array: ";
    //for (int i = 0; i < N; i++)
    //    std::cout << arr[i] << " ";
    //std::cout << std::endl;

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

    // Output the result
    // Output the result with high precision
    std::cout << std::fixed << std::setprecision(9);
    std::cout << duration.count() << std::endl;

    return 0;
}