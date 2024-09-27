#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <time.h>

/**
 * merge - Función que combina dos subarreglos ordenados en un solo arreglo ordenado.
 * @param arr: El arreglo original donde se realiza la combinación.
 * @param lo: Índice de inicio del subarreglo izquierdo.
 * @param mid: Índice del punto medio que separa los dos subarreglos.
 * @param hi: Índice de finalización del subarreglo derecho.
 * 
 * La función merge toma dos subarreglos (de lo a mid y de mid a hi), los compara
 * y combina en un solo subarreglo ordenado. Utiliza arreglos temporales para 
 * almacenar los valores de los subarreglos y luego los fusiona en el arreglo original.
 */
void merge(int* arr, int lo, int mid, int hi) {
    int left_size = mid - lo;   // Tamaño del subarreglo izquierdo A
    int right_size = hi - mid;  // Tamaño del subarreglo derecho B

    // Crear subarreglos A (L) y B (R)
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
            C[k] = A[i];  // Añadir cabeza(A) a C
            i++;          // Eliminar la cabeza de A
        } else {
            C[k] = B[j];  // Añadir cabeza(B) a C
            j++;          // Eliminar la cabeza de B
        }
        k++;
    }

    while (i < left_size) {
        C[k] = A[i];  // Añadir cabeza(A) a C
        i++;          // Eliminar la cabeza de A
        k++;
    }

    while (j < right_size) {
        C[k] = B[j];  // Añadir cabeza(B) a C
        j++;          // Eliminar la cabeza de B
        k++;
    }

    for (k = 0; k < (left_size + right_size); k++) {
        arr[lo + k] = C[k];
    }

    free(A);
    free(B);
    free(C);
}

/**
 * parallel_mergesort - Función de ordenamiento paralelo usando el algoritmo MergeSort.
 * @param arr: El arreglo a ordenar.
 * @param lo: Índice de inicio del subarreglo a ordenar.
 * @param hi: Índice de finalización del subarreglo a ordenar.
 * 
 * Esta función implementa el algoritmo MergeSort de manera recursiva y paralela.
 * Divide el arreglo en dos mitades, ejecuta una de las mitades en un hilo paralelo
 * utilizando cilk_spawn, y espera que ambas tareas terminen usando cilk_sync. Luego,
 * fusiona las mitades ordenadas usando la función merge.
 */
void parallel_mergesort(int* arr, int lo, int hi) {
    if (lo + 1 < hi) {
        int mid = (lo + hi) / 2;

        cilk_spawn parallel_mergesort(arr, lo, mid);  // Crear una tarea para la izquierda
        parallel_mergesort(arr, mid, hi);             // Tarea derecha (continúa sincrónicamente)
        cilk_sync;                                    // Esperar a que la tarea creada finalice

        merge(arr, lo, mid, hi);
    }
}

/**
 * main - Función principal que inicializa el programa, maneja la entrada/salida y ejecuta el ordenamiento.
 * 
 * Esta función:
 * 1. Solicita al usuario el tamaño del array y el número de hilos.
 * 2. Establece el número de hilos mediante una variable de entorno.
 * 3. Crea un array de números aleatorios, lo imprime, lo ordena utilizando parallel_mergesort,
 *    y finalmente muestra el array ordenado junto con el tiempo de ejecución.
 */
int main(int argc, char *argv[]) {
    int n, num_workers;

    // Si se proporcionan argumentos por línea de comandos, utilizarlos
    if (argc == 3) {
        n = atoi(argv[1]);  // Tamaño del array
        num_workers = atoi(argv[2]);  // Número de hilos
    } else {
        // Pedir el tamaño del array si no se proporcionó como argumento
        printf("Ingrese el tamaño del array: ");
        scanf("%d", &n);

        // Pedir el número de hilos si no se proporcionó como argumento
        printf("Ingrese el número de hilos: ");
        scanf("%d", &num_workers);
    }

    // Establecer el número de hilos a través de la variable de entorno
    char env_var[50];
    snprintf(env_var, sizeof(env_var), "CILK_NWORKERS=%d", num_workers);
    putenv(env_var);  // Establecer la variable de entorno para el número de hilos

    // Crear y llenar el array con números aleatorios
    int* arr = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Números aleatorios entre 0 y 999
    }

    // Imprimir el array antes de ordenar
    // printf("Array antes de ser ordenado:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    // Medir el tiempo de inicio usando clock_gettime
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Llamar a la función parallel_mergesort para ordenar el array
    parallel_mergesort(arr, 0, n);

    // Medir el tiempo de finalización
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calcular el tiempo transcurrido
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Imprimir el array después de ordenar
    // printf("Array después de ser ordenado:\n");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    // Mostrar el tiempo total de ejecución
    printf("Tiempo total de ejecución: %f segundos\n", time_taken);

    // Liberar la memoria
    free(arr);
    return 0;
}

