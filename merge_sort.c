#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int* arr, int lo, int mid, int hi) {
    int i, j, k;
    int n1 = mid - lo + 1;
    int n2 = hi - mid;

    int* L = (int*) malloc(n1 * sizeof(int));
    int* R = (int*) malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[lo + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0; j = 0; k = lo;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void parallel_mergesort(int* arr, int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_mergesort(arr, lo, mid);
            #pragma omp section
            parallel_mergesort(arr, mid + 1, hi);
        }

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
        parallel_mergesort(arr, 0, n - 1);
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
