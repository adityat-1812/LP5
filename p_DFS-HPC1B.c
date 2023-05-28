#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 1000

int graph[MAX_NODES][MAX_NODES];
int visited[MAX_NODES];

void dfs(int node) {
    visited[node] = 1;

    #pragma omp parallel for
    for (int i = 0; i < MAX_NODES; i++) {
        if (graph[node][i] && !visited[i]) {
            dfs(i);
        }
    }
}

int main() {
    // initialize the graph
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            graph[i][j] = 0;
        }
        visited[i] = 0;
    }
    graph[0][1] = graph[0][2] = 1;
    graph[1][3] = graph[1][4] = 1;
    graph[2][5] = graph[2][6] = 1;
    graph[3][7] = graph[3][8] = 1;
    graph[4][9] = graph[4][10] = 1;
    graph[5][11] = graph[5][12] = 1;
    graph[6][13] = graph[6][14] = 1;

    // run DFS in parallel
    #pragma omp parallel
    {
        #pragma omp single
        {
            dfs(0);
        }
    }

    // print the visited nodes
    for (int i = 0; i < MAX_NODES; i++) {
        if (visited[i]) {
            printf("%d at thread :%d\n", i, i);
        }
    }
    printf("\n");

    return 0;
}