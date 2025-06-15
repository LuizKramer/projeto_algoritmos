#ifndef ATSP_H
#define ATSP_H

#define INF 9999

extern int **cost;
extern int N;

void loadGraph(const char *filename);
void freeGraph(void);
int calculatePathCost(int *path, int pathLen);
int greedy_atsp(int start, int *path);
int tabu_search(int *path, int pathLen, int tabu_list_size, int max_iterations);
int two_opt(int *path, int pathLen);
void generate_random_path(int *path, int n);
int simulated_annealing_sa2opt(int *bestPath, int pathLen, double initialTemp, double coolingRate, int maxIterations);

#endif
