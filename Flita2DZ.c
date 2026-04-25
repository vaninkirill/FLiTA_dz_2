#include <stdio.h>
#include <stdlib.h>

int isConnected(int **adjMatrix, int n) {
    int visited[100] = {0};
    visited[0] = 1;

    int changed;

    do {
        changed = 0;

        for (int i = 0; i < n; i++) {
            if (visited[i]) {
                for (int j = 0; j < n; j++) {
                    if (adjMatrix[i][j] && !visited[j]) {
                        visited[j] = 1;
                        changed = 1;
                    }
                }
            }
        }

    } while (changed);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) return 0;
    }

    return 1;
}

int main() {
    system("chcp 65001");

    FILE *file = fopen("graph.txt", "r");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return 1;
    }

    int u, v;
    int maxVertex = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "(%d, %d)", &u, &v) == 2) {
            if (u > maxVertex) maxVertex = u;
            if (v > maxVertex) maxVertex = v;
        }
    }

    int n = maxVertex;

    int **adjMatrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        adjMatrix[i] = (int *)calloc(n, sizeof(int));
    }

    rewind(file);

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "(%d, %d)", &u, &v) == 2) {
            adjMatrix[u - 1][v - 1] = 1;
            adjMatrix[v - 1][u - 1] = 1;
        }
    }

    fclose(file);

    printf("Матрица смежности:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", adjMatrix[i][j]);
        }
        printf("\n");
    }

    if (isConnected(adjMatrix, n))
        printf("Граф связный\n");
    else
        printf("Граф несвязный\n");

    FILE *dot = fopen("graph.dot", "w");
    fprintf(dot, "graph G {\n");

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (adjMatrix[i][j]) {
                fprintf(dot, "    %d -- %d;\n", i + 1, j + 1);
            }
        }
    }

    fprintf(dot, "}\n");
    fclose(dot);

    system("dot -Tpng graph.dot -o graph.png");

    printf("Граф сохранён в graph.png\n");

    for (int i = 0; i < n; i++) {
        free(adjMatrix[i]);
    }
    free(adjMatrix);

    return 0;
}