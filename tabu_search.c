#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atsp.h"

#define TABU_LIST_SIZE 1000
#define MAX_ITERATIONS 100000

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Uso: %s <arquivo_atsp> [semente]\n", argv[0]);
        return 1;
    }

    loadGraph(argv[1]);

    int *path = (int *)malloc(N * sizeof(int));
    if (path == NULL) {
        printf("Erro ao alocar memória para o caminho.\n");
        freeGraph();
        return 1;
    }
    int seed = 1749405559;
    srand(seed);
    printf("Semente usada: %d\n", seed);

    generate_random_path(path, N);  // Caminho aleatório inicial

    clock_t start_time = clock();

    // Executa Busca Tabu a partir do caminho aleatório
    int tabuCost = tabu_search(path, N, TABU_LIST_SIZE, MAX_ITERATIONS);

    clock_t end_time = clock();

    printf("\nCaminho otimizado (Busca Tabu): ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]);

    printf("\nCusto após busca tabu: %d", tabuCost);

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\nTempo de execução (busca tabu): %.6f segundos\n", time_taken);

    free(path);
    freeGraph();
    return 0;
}
