#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to generate a random array
void generateRandomArray(int* arr, int n)
{
    srand(42);
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % n;
    }
}

// Function to print an array
void printArray(int* arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Function to swap two elements of an array
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to partition the array
int partition(int* arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Function to perform Quicksort
void quicksort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quicksort(arr, low, pi - 1);
            }
            #pragma omp section
            {
                quicksort(arr, pi + 1, high);
            }
        }
    }
}

// Function to perform parallel Quicksort using OpenMP
void parallelQuicksort(int* arr, int n)
{
    #pragma omp parallel
    {
        #pragma omp single
        {
            quicksort(arr, 0, n - 1);
        }
    }
}

int main()
{
    int n = 10; // Size of array

    int* arr = (int*)malloc(n * sizeof(int));

    generateRandomArray(arr, n);
    printf("Unsorted array: ");
    printArray(arr, n);

    // Measure the time taken for parallel Quicksort
    double start = omp_get_wtime();
    parallelQuicksort(arr, n);
    double end = omp_get_wtime();

    // Print the sorted array and the time taken for sorting
    printf("Sorted array: ");
    printArray(arr, n);
    printf("Time taken (with parallelization): %lf seconds\n", end - start);

    generateRandomArray(arr, n); // Regenerate the unsorted array

    // Measure the time taken for sequential Quicksort
    start = omp_get_wtime();
    quicksort(arr, 0, n - 1);
    end = omp_get_wtime();

    // Print the sorted array and the time taken for sorting
    printf("Sorted array: ");
    printArray(arr, n);
    printf("Time taken (without parallelization): %lf seconds\n", end - start);

    free(arr);
    return 0;
}
