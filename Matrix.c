#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Matrix.h"

// Crée une matrice remplie de 0 [cite: 126]
t_matrix createEmptyMatrix(int rows, int cols) {
    t_matrix m;
    m.rows = rows;
    m.cols = cols;
    m.data = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++) {
        m.data[i] = (float *)calloc(cols, sizeof(float)); // calloc initialise à 0
    }
    return m;
}

// Crée la matrice de transition à partir du graphe [cite: 124-125]
t_matrix createMatrixFromGraph(const liste_adjacence *graph) {
    t_matrix m = createEmptyMatrix(graph->taille, graph->taille);

    for (int i = 0; i < graph->taille; i++) {
        list_t *liste = &graph->liste_arretes[i];
        arrete_t *current = liste->head;
        while (current != NULL) {
            // Attention : les sommets sont 1-based dans le graphe, mais 0-based dans la matrice
            int col = current->sommet_arrive - 1;
            if (col >= 0 && col < graph->taille) {
                m.data[i][col] = current->probabilite;
            }
            current = current->sommet_suivante;
        }
    }
    return m;
}

// Recopie une matrice dans une autre [cite: 127]
void copyMatrix(t_matrix dest, t_matrix src) {
    if (dest.rows != src.rows || dest.cols != src.cols) return;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            dest.data[i][j] = src.data[i][j];
        }
    }
}

// Multiplication de matrices [cite: 128]
void multiplyMatrices(t_matrix m1, t_matrix m2, t_matrix result) {
    if (m1.cols != m2.rows || result.rows != m1.rows || result.cols != m2.cols) {
        printf("Erreur dimensions multiplication\n");
        return;
    }

    // Réinitialiser result à 0 avant calcul
    for(int i=0; i<result.rows; i++)
        for(int j=0; j<result.cols; j++)
            result.data[i][j] = 0.0f;

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m2.cols; j++) {
            for (int k = 0; k < m1.cols; k++) {
                result.data[i][j] += m1.data[i][k] * m2.data[k][j];
            }
        }
    }
}

// Calcule la somme des différences absolues [cite: 129]
float diffMatrix(t_matrix m1, t_matrix m2) {
    float diff = 0.0f;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            diff += fabsf(m1.data[i][j] - m2.data[i][j]);
        }
    }
    return diff;
}

// Extrait la sous-matrice pour une composante connexe [cite: 159-160]
t_matrix subMatrix(t_matrix matrix, t_partition *part, int compo_index) {
    t_classe *classe = &part->classes[compo_index];
    int size = classe->taille;
    t_matrix sub = createEmptyMatrix(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // On récupère les indices globaux des sommets de la classe
            // Attention: sommets stockés en 1-based, donc on fait -1
            int global_row = classe->sommets[i] - 1;
            int global_col = classe->sommets[j] - 1;

            sub.data[i][j] = matrix.data[global_row][global_col];
        }
    }
    return sub;
}

void printMatrix(t_matrix m) {
    for (int i = 0; i < m.rows; i++) {
        printf("( ");
        for (int j = 0; j < m.cols; j++) {
            printf("%.3f ", m.data[i][j]);
        }
        printf(")\n");
    }
}

void freeMatrix(t_matrix *m) {
    if (m->data != NULL) {
        for (int i = 0; i < m->rows; i++) {
            free(m->data[i]);
        }
        free(m->data);
        m->data = NULL;
    }
}

// --- BONUS : Calcul de Période (Code fourni adapté)  ---
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int gcd_array(int *vals, int nbvals) {
    if (nbvals == 0) return 0;
    int result = vals[0];
    for (int i = 1; i < nbvals; i++) {
        result = gcd(result, vals[i]);
    }
    return result;
}

int getPeriod(t_matrix sub_matrix) {
    int n = sub_matrix.rows;
    if (n == 0) return 0;

    int *periods = (int*)malloc(n * n * sizeof(int)); // Allocation large
    int period_count = 0;

    t_matrix power_matrix = createEmptyMatrix(n, n);
    t_matrix result_matrix = createEmptyMatrix(n, n);

    copyMatrix(power_matrix, sub_matrix); // M^1

    // On teste jusqu'à n étapes (borne supérieure simple pour la période)
    for (int cpt = 1; cpt <= n + 10; cpt++) { // +10 par sécurité
        int diag_nonzero = 0;

        // Vérifie si la diagonale a des éléments > 0
        for (int i = 0; i < n; i++) {
            if (power_matrix.data[i][i] > 0.0001f) {
                diag_nonzero = 1;
                break; // Dès qu'un élément diag est >0, c'est une 'boucle' potentielle
            }
        }

        if (diag_nonzero) {
            periods[period_count++] = cpt;
        }

        // M^(k+1) = M^k * M
        multiplyMatrices(power_matrix, sub_matrix, result_matrix);
        copyMatrix(power_matrix, result_matrix);
    }

    int p = gcd_array(periods, period_count);

    free(periods);
    freeMatrix(&power_matrix);
    freeMatrix(&result_matrix);

    return (p == 0) ? 1 : p; // Si p=0, pas de retour, période 1 par défaut ou infini
}