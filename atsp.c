#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "atsp.h"

int **cost = NULL;
int N = 0;

// Carrega o grafo a partir do arquivo
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

void freeGraph(void) {
    for (int i = 0; i < N; i++) {
        free(cost[i]);
    }
    free(cost);
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

int calculatePathCost(int *path, int pathLen) {
    int totalCost = 0;
    for (int i = 0; i < pathLen - 1; i++) {
        totalCost += cost[path[i]][path[i + 1]];
    }
    totalCost += cost[path[pathLen - 1]][path[0]];
    return totalCost;
}

// Busca Tabu
typedef struct {
    int city1;
    int city2;
    int tenure;
} TabuMove;

int tabu_search(int *path, int pathLen, int tabu_list_size, int max_iterations) {
    int bestCost = calculatePathCost(path, pathLen);
    int *bestPath = (int *)malloc(pathLen * sizeof(int));
    memcpy(bestPath, path, pathLen * sizeof(int));

    TabuMove *tabuList = (TabuMove *)malloc(tabu_list_size * sizeof(TabuMove));
    memset(tabuList, 0, tabu_list_size * sizeof(TabuMove));
    int tabuIndex = 0;

    int iteration = 0;
    int no_improve_count = 0;
    int max_no_improve = 100;

    while (iteration < max_iterations && no_improve_count < max_no_improve) {
        int bestNeighborCost = INT_MAX;
        int best_i = -1, best_j = -1;

        
        for (int i = 1; i < pathLen - 1; i++) {
            for (int j = i + 1; j < pathLen; j++) {
                // Calcula o novo custo após a troca
                int temp = path[i];
                path[i] = path[j];
                path[j] = temp;
                int newCost = calculatePathCost(path, pathLen);

                // Verifica se é tabu
                int isTabu = 0;
                for (int k = 0; k < tabu_list_size; k++) {
                    if ((tabuList[k].city1 == path[i] && tabuList[k].city2 == path[j]) ||
                        (tabuList[k].city1 == path[j] && tabuList[k].city2 == path[i])) {
                        isTabu = 1;
                        break;
                    }
                }


                if (!isTabu || newCost < bestCost) {
                    if (newCost < bestNeighborCost) {
                        bestNeighborCost = newCost;
                        best_i = i;
                        best_j = j;
                    }
                }

                // Desfaz a troca para testar o próximo par
                temp = path[i];
                path[i] = path[j];
                path[j] = temp;
            }
        }

        // Atualiza o melhor caminho
        if (best_i != -1 && best_j != -1) {
            // Aplica a troca
            int temp = path[best_i];
            path[best_i] = path[best_j];
            path[best_j] = temp;

            // Atualiza a lista tabu
            tabuList[tabuIndex].city1 = path[best_i];
            tabuList[tabuIndex].city2 = path[best_j];
            tabuList[tabuIndex].tenure = iteration + tabu_list_size;
            tabuIndex = (tabuIndex + 1) % tabu_list_size;

            // Verifica se é o melhor caminho encontrado até agora
            if (bestNeighborCost < bestCost) {
                bestCost = bestNeighborCost;
                memcpy(bestPath, path, pathLen * sizeof(int));
                no_improve_count = 0;
            } else {
                no_improve_count++;
            }
        }

        iteration++;
    }

    // Restaura o melhor caminho encontrado
    memcpy(path, bestPath, pathLen * sizeof(int));
    free(bestPath);
    free(tabuList);

    return bestCost;
}
