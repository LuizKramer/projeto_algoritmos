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
    if (path == NULL) {
        printf("Erro ao alocar memória para o caminho.\n");
        return 1;
    }

    clock_t start_time = clock();
    int totalCost = greedy_atsp(0, path);
    clock_t end_time = clock();

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\nCaminho encontrado: ");
    for (int i = 0; i < N; i++) {
        printf("%d->", path[i]);
    }
    printf("%d\n", path[0]); // retorno ao início

    printf("\nCusto Total: %d\n", totalCost);
    printf("Tempo de Execução: %.6f segundos\n", time_taken);

    free(path);
    freeGraph();
    return 0;
}
