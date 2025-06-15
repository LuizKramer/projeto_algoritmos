#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "atsp.h"

#define TABU_LIST_SIZE 200
#define MAX_ITERATIONS 10000

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_atsp>\n", argv[0]);
        return 1;
    }

    loadGraph(argv[1]);
    int *path = malloc(N * sizeof(int));
    int *backup = malloc(N * sizeof(int));
    if (!path || !backup) {
        printf("Erro de memória.\n");
        return 1;
    }

    int seed = 15102000;
    printf("Semente usada: %d\n", seed);

    // --- GREEDY ---
    clock_t start = clock();
    int greedyCost = greedy_atsp(0, path);
    clock_t end = clock();
    printf("\n[Greedy]\n");
    printf("Custo: %d\n", greedyCost);
    printf("Tempo: %.6f s\n", (double)(end - start) / CLOCKS_PER_SEC);
    memcpy(backup, path, N * sizeof(int)); // salva para Greedy + Tabu + 2opt

    // --- 2-OPT (após caminho aleatório) ---
    srand(seed);
    generate_random_path(path, N);
    start = clock();
    int optCost = two_opt(path, N);
    end = clock();
    printf("\n[2-opt (aleatório)]\n");
    printf("Custo: %d\n", optCost);
    printf("Tempo: %.6f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // --- TABU SEARCH (aleatório) ---
    int start1, end1;
    srand(seed);
    generate_random_path(path, N);
    start = clock();
    start1 = clock();
    int tabuCost = tabu_search(path, N, N/4, MAX_ITERATIONS);
    end = clock();
    int finalOptCost = two_opt(path, N);
    end1 = clock();
    printf("\n[Tabu Search (aleatório)]\n");
    printf("Custo: %d\n", tabuCost);
    printf("Tempo: %.6f s\n", (double)(end - start) / CLOCKS_PER_SEC);-

    printf("\n[Tabu + 2-opt (aleatório)]\n");
    printf("Custo: %d\n", finalOptCost);
    printf("Tempo: %.6f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    // --- GREEDY + TABU + 2-OPT ---
    memcpy(path, backup, N * sizeof(int)); // começa do caminho greedy
    start = clock();
    int tabuCostG = tabu_search(path, N, TABU_LIST_SIZE, MAX_ITERATIONS);
    int optCostG = two_opt(path, N);
    end = clock();
    printf("\n[Greedy + Tabu + 2-opt]\n");
    printf("Custo: %d\n", optCostG);
    printf("Tempo: %.6f s\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(path);
    free(backup);
    freeGraph();
    return 0;
}

