#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "atsp.h"

int **cost = NULL;
int N = 0;

typedef struct {
    int city1;
    int city2;
    int tenure; // até qual iteração é tabu
} TabuMove;

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

void generate_random_path(int *path, int n) {
    for (int i = 0; i < n; i++) {
        path[i] = i;
    }
    // Embaralhamento Fisher-Yates
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
}

int tabu_search(int *path, int pathLen, int tabu_list_size, int max_iterations) {
    int bestCost = calculatePathCost(path, pathLen);
    int currentCost = bestCost;
    int *bestPath = (int *)malloc(pathLen * sizeof(int));
    if (!bestPath) {
        printf("Erro ao alocar memória para bestPath.\n");
        return bestCost;
    }
    memcpy(bestPath, path, pathLen * sizeof(int));

    TabuMove *tabuList = (TabuMove *)malloc(tabu_list_size * sizeof(TabuMove));
    if (!tabuList) {
        printf("Erro ao alocar memória para tabuList.\n");
        free(bestPath);
        return bestCost;
    }

    int tabuIndex = 0;
    int iteration = 0;
    int noImprovementCount = 0;
    int maxNoImprovement = 1000;

    while (iteration < max_iterations && noImprovementCount < maxNoImprovement) {
        int bestNeighborCost = INT_MAX;
        int best_i = -1, best_j = -1;

        for (int i = 1; i < pathLen - 1; i++) {
            for (int j = i + 1; j < pathLen; j++) {
                // Candidatos para troca
                int c1 = path[i];
                int c2 = path[j];

                // Verifica se está na tabu list
                int isTabu = 0;
                for (int k = 0; k < tabu_list_size; k++) {
                    if (((tabuList[k].city1 == c1 && tabuList[k].city2 == c2) ||
                         (tabuList[k].city1 == c2 && tabuList[k].city2 == c1)) &&
                         iteration < tabuList[k].tenure) {
                        isTabu = 1;
                        break;
                    }
                }

                // Aplica swap temporário
                int temp = path[i];
                path[i] = path[j];
                path[j] = temp;

                int neighborCost = calculatePathCost(path, pathLen);

                // Desfaz o swap
                temp = path[i];
                path[i] = path[j];
                path[j] = temp;

                // Critério de aspiração
                if (!isTabu || neighborCost < bestCost) {
                    if (neighborCost < bestNeighborCost) {
                        bestNeighborCost = neighborCost;
                        best_i = i;
                        best_j = j;
                    }
                }
            }
        }

        // Aplica a melhor troca
        if (best_i != -1 && best_j != -1) {
            int temp = path[best_i];
            path[best_i] = path[best_j];
            path[best_j] = temp;

            currentCost = bestNeighborCost;

            // Atualiza tabu list com valores antes da troca
            tabuList[tabuIndex].city1 = path[best_j]; // valor antigo em best_i
            tabuList[tabuIndex].city2 = path[best_i]; // valor antigo em best_j
            tabuList[tabuIndex].tenure = iteration + tabu_list_size;

            tabuIndex = (tabuIndex + 1) % tabu_list_size;

            if (currentCost < bestCost) {
                bestCost = currentCost;
                memcpy(bestPath, path, pathLen * sizeof(int));
                noImprovementCount = 0;
            } else {
                noImprovementCount++;
            }
        }

        iteration++;
    }

    memcpy(path, bestPath, pathLen * sizeof(int));
    free(bestPath);
    free(tabuList);
    return bestCost;
}

int two_opt(int *path, int pathLen) {
    int improved = 1;
    int bestCost = calculatePathCost(path, pathLen);

    while (improved) {
        improved = 0;

        for (int i = 1; i < pathLen - 1; i++) {
            for (int k = i + 1; k < pathLen; k++) {
                // Cria novo caminho com os elementos entre i e k invertidos
                int newPath[pathLen];
                for (int j = 0; j < i; j++) newPath[j] = path[j];
                int dec = 0;
                for (int j = i; j <= k; j++) newPath[j] = path[k - dec++];
                for (int j = k + 1; j < pathLen; j++) newPath[j] = path[j];

                int newCost = calculatePathCost(newPath, pathLen);
                if (newCost < bestCost) {
                    memcpy(path, newPath, pathLen * sizeof(int));
                    bestCost = newCost;
                    improved = 1;
                    goto loop_restart; // Reinicia as buscas após melhoria
                }
            }
        }

    loop_restart:;
    }

    return bestCost;
}
