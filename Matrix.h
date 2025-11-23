#ifndef MATRIX_H
#define MATRIX_H

#include "markov.h" // Pour t_partition et liste_adjacence

typedef struct {
    int rows;
    int cols;
    float **data;
} t_matrix;

// --- Etape 1 : Calculs matriciels de base ---
t_matrix createEmptyMatrix(int rows, int cols);
t_matrix createMatrixFromGraph(const liste_adjacence *graph);
void copyMatrix(t_matrix dest, t_matrix src);
void multiplyMatrices(t_matrix m1, t_matrix m2, t_matrix result);
float diffMatrix(t_matrix m1, t_matrix m2);
void printMatrix(t_matrix m);
void freeMatrix(t_matrix *m);

// --- Etape 2 : Sous-matrices et partitions ---
t_matrix subMatrix(t_matrix matrix, t_partition *part, int compo_index);

// --- Bonus : Périodicité ---
int getPeriod(t_matrix sub_matrix);

#endif // MATRIX_H