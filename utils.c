#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "list.h"

static char *getID(int i) {
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
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
        perror("Could not open file for reading");
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
        perror("échec de création du graphe");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int line_count = 0;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        line_count++;
        printf("DEBUG readGraph ligne %d: %d %d %.3f\n", line_count, depart, arrivee, proba);

        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            printf("Warning: Arête (%d, %d) ignorée - hors limites\n", depart, arrivee);
            continue;
        }

        // IMPORTANT: Vérifier qu'on utilise le bon index
        list_add_cell(&graphe->liste_arretes[depart - 1], depart, arrivee, proba);
    }

    printf("DEBUG: %d lignes d'arêtes lues\n", line_count);
    fclose(file);
    return graphe;
}

//liste_adjacence *readGraph(const char *filename) {
//    FILE *file = fopen(filename, "rt"); // read-only, text
//    int nbvert, depart, arrivee;
//    float proba;
//    // declarer la variable pour la liste d’adjacence
//    liste_adjacence *graphe;
//    if (file == NULL) {
//        perror("Could not open file for reading");
//        exit(EXIT_FAILURE);
//    }
//    // first line contains number of vertices
//    if (fscanf(file, "%d", &nbvert) != 1) {
//        perror("Could not read number of vertices");
//        exit(EXIT_FAILURE);
//    }
//    // Initialiser une liste d’adjacence vide à partir du nombre de sommets
//    graphe = create_graph(nbvert);
//    if (graphe == NULL)
//    {
//        perror("échec de création du graphe dans la fonction lecture des données du graphe dans un fiche.");
//        fclose(file);
//        exit(EXIT_FAILURE);
//    }
//
//    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
//        // on obtient, pour chaque ligne du fichier les valeurs
//        // depart, arrivee, et proba
//        // Ajouter l’arête qui va de ‘depart’ à ‘arrivée’ avec la
//        // probabilité ‘proba’ dans la liste d’adjacence
//
//        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
//            printf("Warning: Arête (%d, %d) ignorée - hors limites\n", depart, arrivee);
//            continue;
//        }
//
//        list_add_cell(&graphe->liste_arretes[depart - 1], depart, arrivee, proba);
//    }
//    fclose(file);
//    return graphe;
//}
