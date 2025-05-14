#include <stdio.h>
#include <time.h>
#include "atsp.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_atsp>\n", argv[0]);
        return 1;
    }

    loadGraph(argv[1]);

    clock_t start_time = clock();
    int totalCost = greedy_atsp(0);
    clock_t end_time = clock();

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\nCusto Total: %d\n", totalCost);
    printf("Tempo de Execução: %.6f segundos\n", time_taken);

    freeGraph();
    return 0;
}
