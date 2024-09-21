#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Merge two subarrays A and B into a new array C, and copy the result back to arr
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

    // Create the new list C to store the merged result
    int* C = (int*) malloc((left_size + right_size) * sizeof(int));

    int i = 0, j = 0, k = 0;
    
    // While A is not empty and B is not empty
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

    // While A is not empty, append the rest of A to C
    while (i < left_size) {
        C[k] = A[i];  // append head(A) to C
        i++;          // drop the head of A
        k++;
    }

    // While B is not empty, append the rest of B to C
    while (j < right_size) {
        C[k] = B[j];  // append head(B) to C
        j++;          // drop the head of B
        k++;
    }

    // Copy the merged result back to the original array arr
    for (k = 0; k < (left_size + right_size); k++) {
        arr[lo + k] = C[k];
    }

    // Free allocated memory for subarrays A, B, and the merged list C
    free(A);
    free(B);
    free(C);
}


void parallel_mergesort(int* arr, int lo, int hi) {
    // Match the exact condition from the provided algorithm
    if (lo + 1 < hi) {
        int mid = (lo + hi) / 2;

        // Parallelize the two recursive calls using OpenMP
        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_mergesort(arr, lo, mid);
            
            #pragma omp section
            parallel_mergesort(arr, mid, hi);
        }

        // Merge the two halves after they have been sorted
        merge(arr, lo, mid, hi);
    }
}

int main() {
    int n;

    // Pedir el tamaño del array al usuario
    printf("Ingrese el tamaño del array: ");
    scanf("%d", &n);

    // Crear y llenar el array con números aleatorios
    int* arr = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Números aleatorios entre 0 y 999
    }

    // Imprimir el array antes de ser ordenado
    printf("Array antes de ser ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Llamar a la función parallel_mergesort para ordenar el array
    #pragma omp parallel
    {
        #pragma omp single
        parallel_mergesort(arr, 0, n);
    }

    // Imprimir el array después de ser ordenado
    printf("Array después de ser ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Liberar la memoria
    free(arr);
    return 0;
}
