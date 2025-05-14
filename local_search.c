#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atsp.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_atsp>\n", argv[0]);
        return 1;
    }

    loadGraph(argv[1]);

    int *path = (int *)malloc(N * sizeof(int));

    clock_t start_time = clock();
    int greedyCost = greedy_atsp(0, path);
    clock_t greedy_time = clock();

    int improvedCost = local_search_2opt(path, N);
    clock_t end_time = clock();

    printf("\nCaminho otimizado: ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]);

    printf("\nCusto inicial (guloso): %d", greedyCost);
    printf("\nCusto após busca local: %d", improvedCost);

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\nTempo total: %.6f segundos\n", time_taken);

    free(path);
    freeGraph();
    return 0;
}
