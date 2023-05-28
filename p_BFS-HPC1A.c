#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 10000

int n;
int adj_list[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];
int global_queue[MAX_NODES];
int queue_size;

void parallel_bfs(int start) {
    visited[start] = 1;
    global_queue[0] = start;
    queue_size = 1;

    while (queue_size > 0) {
        int num_threads = omp_get_max_threads();
        int chunk_size = (queue_size + num_threads - 1) / num_threads;
        int local_queue[MAX_NODES];
        int local_queue_size = 0;

        #pragma omp parallel for shared(local_queue, local_queue_size)
        for (int i = 0; i < queue_size; i++) {
            int node = global_queue[i];

            for (int j = 0; j < n; j++) {
                if (adj_list[node][j] && !visited[j]) {
                    visited[j] = 1;
                    local_queue[local_queue_size++] = j;
                }
            }
        }

        queue_size = 0;

        #pragma omp parallel for
        for (int i = 0; i < num_threads; i++) {
            int start = i * chunk_size;
            int end = start + chunk_size;
            if (end > local_queue_size) {
                end = local_queue_size;
            }

            for (int j = start; j < end; j++) {
                global_queue[queue_size++] = local_queue[j];
            }
        }
    }
}

int main() {
    int start_node;

    printf("Enter the number of nodes: ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix: \n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &adj_list[i][j]);
        }
    }

    printf("Enter the starting node: ");
    scanf("%d", &start_node);

    parallel_bfs(start_node);

    printf("The BFS traversal order is: ");
    for (int i = 0; i < n; i++) {
        if (visited[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    return 0;
}
