#ifndef ATSP_H
#define ATSP_H

#define INF 9999

extern int **cost;
extern int N;

void loadGraph(const char *filename);
int greedy_atsp(int start);
void freeGraph(void);

#endif  