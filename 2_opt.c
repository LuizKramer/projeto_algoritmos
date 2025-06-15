#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "atsp.h"

extern int **cost;
extern int N;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.atsp>\n", argv[0]);
        return EXIT_FAILURE;
    }

    loadGraph(argv[1]);

    int *path = (int *)malloc(N * sizeof(int));
    if (!path) {
        printf("Erro de alocação de memória\n");
        return EXIT_FAILURE;
    }

    // Começa a medir tempo
    clock_t start = clock();

    // Gera solução inicial e aplica 2-opt
       int seed = 15102000;
    srand(seed);
    printf("Semente usada: %d\n", seed);

    generate_random_path(path, N);  // Caminho aleatório inicial
    int optCost = two_opt(path, N);

    // Para de medir tempo
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Resultados
    printf("Custo final após 2-opt: %d\n", optCost);
    printf("Tempo de execução: %.6f segundos\n", time_spent);

    printf("Caminho final:\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", path[i]);
    }
    printf("%d\n", path[0]); // para fechar o ciclo

    free(path);
    freeGraph();
    return 0;
}
