#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "list.h"

static char *getID(int i) {

    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }


    for (int j = 0; j < index; j++) {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

liste_adjacence *readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;
    liste_adjacence *graphe;

    if (file == NULL) {
        perror("Je ne peux pass ouvrir le fichier pour le lire");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    printf("DEBUG: Nombre de sommets = %d\n", nbvert);

    graphe = create_graph(nbvert);
    if (graphe == NULL) {
        perror("echec de creation du graphe");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int line_count = 0;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        line_count++;
        printf("DEBUG readGraph ligne %d: %d %d %.3f\n", line_count, depart, arrivee, proba);

        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            printf("Warning: Arete (%d, %d) ignoree - hors limites\n", depart, arrivee);
            continue;
        }


        list_add_cell(&graphe->liste_arretes[depart - 1], depart, arrivee, proba);
    }

    printf("DEBUG: %d lignes d'aretes lues\n", line_count);
    fclose(file);
    return graphe;
}
