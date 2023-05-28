#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    int min = arr[0];
    int max = arr[0];
    int sum = 0;

    #pragma omp parallel for reduction(min:min) reduction(max:max) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        if (arr[i] > max) {
            max = arr[i];
        }
        sum += arr[i];

        // Print out the current thread ID and the number of threads
        printf("Thread %d out of %d threads is working on element %d\n", omp_get_thread_num(), omp_get_num_threads(), i);
    }

    double average = (double) sum / n;

    printf("Min value: %d\n", min);
    printf("Max value: %d\n", max);
    printf("Sum value: %d\n", sum);
    printf("Average value: %f\n", average);

    return 0;
}