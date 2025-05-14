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
int greedy_atsp(int start, int *path) {
    int min;
    int currentCity = start;
    int visited[N];
    int totalCost = 0;
    int idx = 0;

    for (int i = 0; i < N; i++) visited[i] = 0;

    visited[currentCity] = 1;
    path[idx++] = currentCity;

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
        path[idx++] = nextCity;
    }

    totalCost += cost[currentCity][start];

    return totalCost;
}


void freeGraph(void) {
    for (int i = 0; i < N; i++) {
        free(cost[i]);
    }
    free(cost);
}

int calculatePathCost(int *path, int pathLen) {
    int totalCost = 0;
    for (int i = 0; i < pathLen - 1; i++) {
        totalCost += cost[path[i]][path[i + 1]];
    }
    totalCost += cost[path[pathLen - 1]][path[0]]; // volta ao início
    return totalCost;
}

int local_search_2opt(int *path, int pathLen) {
    int improved = 1;

    while (improved) {
        improved = 0;
        for (int i = 1; i < pathLen - 1; i++) {
            for (int j = i + 1; j < pathLen; j++) {
                // Evita troca de início
                if (j - i == 1) continue;

                // Calcula o custo atual
                int A = path[i - 1], B = path[i];
                int C = path[j], D = path[(j + 1) % pathLen];

                int before = cost[A][B] + cost[C][D];
                int after  = cost[A][C] + cost[B][D];

                if (after < before) {
                    // Inverte segmento [i, j]
                    while (i < j) {
                        int temp = path[i];
                        path[i] = path[j];
                        path[j] = temp;
                        i++;
                        j--;
                    }
                    improved = 1;
                    break;
                }
            }
            if (improved) break;
        }
    }

    return calculatePathCost(path, pathLen);
}
