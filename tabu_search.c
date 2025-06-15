#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atsp.h"

#define TABU_LIST_SIZE 10000
#define MAX_ITERATIONS 1000000

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

    int seed = 15102000;
    srand(seed);
    printf("Semente usada: %d\n", seed);

    generate_random_path(path, N);  // Caminho aleatório inicial

    // Tempo de execução da busca tabu
    clock_t start_tabu = clock();
    int tabuCost = tabu_search(path, N, TABU_LIST_SIZE, MAX_ITERATIONS);
    clock_t end_tabu = clock();

    printf("\nCaminho otimizado (Busca Tabu): ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]);
    printf("Custo após busca tabu: %d\n", tabuCost);

    double tabu_time = ((double)(end_tabu - start_tabu)) / CLOCKS_PER_SEC;
    printf("Tempo de execução (busca tabu): %.6f segundos\n", tabu_time);

    // Tempo de execução da 2-opt após tabu
    clock_t start_total = start_tabu; // Início do total é o mesmo da tabu
    int optCost = two_opt(path, N);
    clock_t end_total = clock(); // Final do total é após o 2-opt

    printf("\nCaminho otimizado (2-opt após busca tabu): ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]);
    printf("Custo final após 2-opt: %d\n", optCost);

    double total_time = ((double)(end_total - start_total)) / CLOCKS_PER_SEC;
    printf("Tempo total (busca tabu + 2-opt): %.6f segundos\n", total_time);

    free(path);
    freeGraph();
    return 0;
}
