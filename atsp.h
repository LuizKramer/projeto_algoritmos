#ifndef ATSP_H
#define ATSP_H

#define INF 9999

extern int **cost;
extern int N;

void loadGraph(const char *filename);
void freeGraph(void);
int calculatePathCost(int *path, int pathLen);
int local_search_2opt(int *path, int pathLen);
int greedy_atsp(int start, int *path); // novo parâmetro 'path'

#endif
