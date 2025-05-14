#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atsp.h"

int **cost = NULL;
int N = 0;

void loadGraph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    int row = 0, col = 0;
    int readingMatrix = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "DIMENSION")) {
            sscanf(line, "DIMENSION: %d", &N);
            break;
        }
    }

    cost = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        cost[i] = (int *)malloc(N * sizeof(int));
    }

    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "EDGE_WEIGHT_SECTION")) {
            readingMatrix = 1;
            continue;
        }
        if (strstr(line, "EOF")) break;

        if (readingMatrix) {
            char *token = strtok(line, " \t\n");
            while (token != NULL) {
                cost[row][col] = atoi(token);
                col++;
                if (col == N) {
                    row++;
                    col = 0;
                }
                token = strtok(NULL, " \t\n");
            }
        }
    }

    fclose(file);
}

int greedy_atsp(int start) {
    int min;
    int currentCity = start;
    int visited[N];
    int totalCost = 0;

    for (int i = 0; i < N; i++) visited[i] = 0;
    visited[currentCity] = 1;

    printf("\nCaminho Percorrido: %d->", start);

    for (int i = 0; i < N - 1; i++) {
        min = INF;
        int nextCity = -1;

        for (int j = 0; j < N; j++) {
            if (!visited[j] && cost[currentCity][j] < min) {
                min = cost[currentCity][j];
                nextCity = j;
            }
        }

        if (nextCity == -1) {
            printf("\nErro: Não foi possível encontrar o próximo caminho.\n");
            return -1;
        }

        visited[nextCity] = 1;
        totalCost += min;
        currentCity = nextCity;

        printf("%d->", nextCity);
    }

    totalCost += cost[currentCity][start];
    printf("%d (custo: %d)\n", start, cost[currentCity][start]);

    return totalCost;
}

void freeGraph(void) {
    for (int i = 0; i < N; i++) {
        free(cost[i]);
    }
    free(cost);
}
