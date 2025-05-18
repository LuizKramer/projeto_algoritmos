#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atsp.h"

#define TABU_LIST_SIZE 50
#define MAX_ITERATIONS 1000

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_atsp>\n", argv[0]);
        return 1;
    }

    // Carrega o grafo do arquivo
    loadGraph(argv[1]);

    // Aloca memória para o caminho
    int *path = (int *)malloc(N * sizeof(int));
    if (path == NULL) {
        printf("Erro ao alocar memória para o caminho.\n");
        freeGraph();
        return 1;
    }

    // Inicia a solução com o método guloso
    clock_t start_time = clock();
    int greedyCost = greedy_atsp(0, path);
    clock_t greedy_time = clock();

    // Aplica a busca tabu
    int tabuCost = tabu_search(path, N, TABU_LIST_SIZE, MAX_ITERATIONS);
    clock_t end_time = clock();

    // Exibe o caminho otimizado
    printf("\nCaminho otimizado (Busca Tabu): ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]);

    // Exibe os custos
    printf("\nCusto inicial (guloso): %d", greedyCost);
    printf("\nCusto após busca tabu: %d", tabuCost);

    // Calcula e exibe o tempo total
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double greedy_time_taken = ((double)(greedy_time - start_time)) / CLOCKS_PER_SEC;
    double tabu_time = ((double)(end_time - greedy_time)) / CLOCKS_PER_SEC;

    printf("\nTempo total: %.6f segundos", time_taken);
    printf("\nTempo guloso: %.6f segundos", greedy_time_taken);
    printf("\nTempo busca tabu: %.6f segundos\n", tabu_time);

    // Libera a memória
    free(path);
    freeGraph();
    return 0;
}
