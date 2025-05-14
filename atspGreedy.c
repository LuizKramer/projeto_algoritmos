#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 9999

int **cost;
int N;

void loadGraph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    int row = 0, col = 0;
    int readingMatrix = 0;

    // Lendo a dimensão do grafo
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "DIMENSION")) {
            sscanf(line, "DIMENSION: %d", &N);
            break;
        }
    }

    // Alocando memória para a matriz de custos
    cost = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        cost[i] = (int *)malloc(N * sizeof(int));
    }

    // Recomeçando a leitura para encontrar a seção de pesos
    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "EDGE_WEIGHT_SECTION")) {
            readingMatrix = 1;
            continue;
        }
        if (strstr(line, "EOF")) break;

        // Preenchendo a matriz de custos
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

int isNotVisited(int visited[], int city, int visitedCount) {
    for (int i = 0; i < visitedCount; i++) {
        if (visited[i] == city) {
            return 0;
        }
    }
    return 1;
}

int greedy_atsp(int start) {
    int min;
    int currentCity = start;
    int visited[N];
    int totalCost = 0;
    int visitedCount = 1;

    // Inicializa o vetor de visitados
    for (int i = 0; i < N; i++) visited[i] = 0;
    visited[currentCity] = 1;

    printf("Cidades Visitadas: %d->", start);

    for (int i = 0; i < N - 1; i++) {
        min = INF;
        int nextCity = -1;

        // Encontra a próxima cidade com o menor custo
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

        // Marca como visitada e atualiza o custo total
        visited[nextCity] = 1;
        totalCost += min;
        currentCity = nextCity;
        visitedCount++;

        // Imprime a cidade visitada
        printf("%d->", nextCity);
    }

    // Fecha o ciclo voltando para a cidade inicial
    totalCost += cost[currentCity][start];
    printf("%d\n", start);

    return totalCost;
}



void freeGraph() {
    for (int i = 0; i < N; i++) {
        free(cost[i]);
    }
    free(cost);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo_atsp>\n", argv[0]);
        return 1;
    }

    loadGraph(argv[1]);

    printf("Custo Total: %d\n", greedy_atsp(0));

    freeGraph();
    return 0;
}
