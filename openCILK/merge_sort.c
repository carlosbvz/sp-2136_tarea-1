#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <time.h>

void merge(int* arr, int lo, int mid, int hi) {
    int left_size = mid - lo;   // Size of the left subarray A
    int right_size = hi - mid;  // Size of the right subarray B

    // Create subarrays A (L) and B (R)
    int* A = (int*) malloc(left_size * sizeof(int));
    int* B = (int*) malloc(right_size * sizeof(int));

    for (int i = 0; i < left_size; i++)
        A[i] = arr[lo + i];
    for (int j = 0; j < right_size; j++)
        B[j] = arr[mid + j];

    int* C = (int*) malloc((left_size + right_size) * sizeof(int));

    int i = 0, j = 0, k = 0;
    
    while (i < left_size && j < right_size) {
        if (A[i] <= B[j]) {
            C[k] = A[i];  // append head(A) to C
            i++;          // drop the head of A
        } else {
            C[k] = B[j];  // append head(B) to C
            j++;          // drop the head of B
        }
        k++;
    }

    while (i < left_size) {
        C[k] = A[i];  // append head(A) to C
        i++;          // drop the head of A
        k++;
    }

    while (j < right_size) {
        C[k] = B[j];  // append head(B) to C
        j++;          // drop the head of B
        k++;
    }

    for (k = 0; k < (left_size + right_size); k++) {
        arr[lo + k] = C[k];
    }

    free(A);
    free(B);
    free(C);
}

void parallel_mergesort(int* arr, int lo, int hi) {
    if (lo + 1 < hi) {
        int mid = (lo + hi) / 2;

        cilk_spawn parallel_mergesort(arr, lo, mid);  // Spawn the left task
        parallel_mergesort(arr, mid, hi);             // Right task (continues synchronously)
        cilk_sync;                                    // Wait for the spawned task to complete

        merge(arr, lo, mid, hi);
    }
}

int main() {
    int n, num_workers;

    // Ask for the array size from the user
    printf("Ingrese el tamaño del array: ");
    scanf("%d", &n);

    // Ask for the number of workers from the user
    printf("Ingrese el número de trabajadores (hilos): ");
    scanf("%d", &num_workers);

    // Set the number of workers through the environment variable
    char env_var[50];
    snprintf(env_var, sizeof(env_var), "CILK_NWORKERS=%d", num_workers);
    putenv(env_var);  // Set the environment variable for number of workers

    // Create and fill the array with random numbers
    int* arr = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Random numbers between 0 and 999
    }

    // Print the array before sorting
    printf("Array antes de ser ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Measure start time using clock_gettime
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Call the parallel_mergesort function to sort the array
    parallel_mergesort(arr, 0, n);

    // Measure end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Print the array after sorting
    printf("Array después de ser ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Show total execution time
    printf("Tiempo total de ejecución: %f segundos\n", time_taken);

    // Free the memory
    free(arr);
    return 0;
}
