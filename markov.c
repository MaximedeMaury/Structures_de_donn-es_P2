#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "markov.h"

int is_markov_graph(const liste_adjacence *graph) {
    if (graph == NULL) return 0;

    for (int i = 0; i < graph->taille; i++) {
        float sum = 0.0f;
        list_t *liste = &graph->liste_arretes[i];
        arrete_t *current = liste->head;

        while (current != NULL) {
            sum += current->probabilite;
            current = current->sommet_suivante;
        }

        // Vérifier si la somme est entre 0.99 et 1.01 (tolérance pour les erreurs d'arrondi)
        if (fabs(sum - 1.0f) > 0.01f) {
            printf("Erreur: la somme des probabilités du sommet %d est %.3f\n", i + 1, sum);
            return 0;
        }
    }
    return 1;
}

void verify_markov_property(const liste_adjacence *graph) {
    if (is_markov_graph(graph)) {
        printf("Le graphe est un graphe de Markov\n");
    } else {
        printf("Le graphe n'est pas un graphe de Markov\n");
    }
}

static char *getID(int i) {
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

void generate_mermaid_graph(const liste_adjacence *graph, const char *filename) {
    if (graph == NULL || filename == NULL) return;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier Mermaid");
        return;
    }

    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "    layout: elk\n");
    fprintf(file, "    theme: neo\n");
    fprintf(file, "    look: neo\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart LR\n");

    // Déclarer TOUS les sommets avec leurs numéros comme IDs
    for (int i = 0; i < graph->taille; i++) {
        fprintf(file, "    N%d((%d))\n", i + 1, i + 1);
    }
    fprintf(file, "\n");

    // Ajouter les arêtes en utilisant les numéros comme IDs
    for (int i = 0; i < graph->taille; i++) {
        list_t *liste = &graph->liste_arretes[i];
        arrete_t *current = liste->head;

        while (current != NULL) {
            fprintf(file, "    N%d-->|%.3f|N%d\n",
                    current->sommet_depart,
                    current->probabilite,
                    current->sommet_arrive);
            current = current->sommet_suivante;
        }
    }

    fclose(file);
    printf("Fichier Mermaid généré: %s\n", filename);
}

//void generate_mermaid_graph(const liste_adjacence *graph, const char *filename) {
//    if (graph == NULL || filename == NULL) return;
//
//    FILE *file = fopen(filename, "w");
//    if (file == NULL) {
//        perror("Erreur d'ouverture du fichier Mermaid");
//        return;
//    }
//
//    fprintf(file, "---\n");
//    fprintf(file, "config:\n");
//    fprintf(file, "    layout: elk\n");
//    fprintf(file, "    theme: neo\n");
//    fprintf(file, "    look: neo\n");
//    fprintf(file, "---\n");
//    fprintf(file, "flowchart LR\n");
//
//    // Déclarer TOUS les sommets d'abord (même ceux sans arêtes)
//    for (int i = 0; i < graph->taille; i++) {
//        fprintf(file, "    %s((%d))\n", getID(i + 1), i + 1);
//    }
//    fprintf(file, "\n");
//
//    // Ajouter les arêtes avec une indentation correcte
//    for (int i = 0; i < graph->taille; i++) {
//        list_t *liste = &graph->liste_arretes[i];
//        arrete_t *current = liste->head;
//
//        while (current != NULL) {
//            fprintf(file, "    %s-->|%.3f|%s\n",
//                    getID(current->sommet_depart),
//                    current->probabilite,
//                    getID(current->sommet_arrive));
//            current = current->sommet_suivante;
//        }
//    }
//
//    fclose(file);
//    printf("Fichier Mermaid généré: %s\n", filename);
//}

void generate_mermaid_hasse(const t_partition *partition, const liste_adjacence *graph, const char *filename) {
    if (partition == NULL || graph == NULL || filename == NULL) return;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier Hasse");
        return;
    }

    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "    layout: elk\n");
    fprintf(file, "    theme: neo\n");
    fprintf(file, "    look: neo\n");
    fprintf(file, "---\n");
    fprintf(file, "flowchart TD\n");

    // Déclarer les classes
    for (int i = 0; i < partition->taille; i++) {
        fprintf(file, "%s[", partition->classes[i].nom);
        for (int j = 0; j < partition->classes[i].taille; j++) {
            fprintf(file, "%d", partition->classes[i].sommets[j]);
            if (j < partition->classes[i].taille - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "]\n");
    }
    fprintf(file, "\n");

    // Trouver les liens entre classes
    int *class_map = (int *)malloc((graph->taille + 1) * sizeof(int));
    for (int i = 0; i < partition->taille; i++) {
        for (int j = 0; j < partition->classes[i].taille; j++) {
            class_map[partition->classes[i].sommets[j]] = i;
        }
    }

    // Parcourir toutes les arêtes pour trouver les liens entre classes
    for (int i = 0; i < graph->taille; i++) {
        list_t *liste = &graph->liste_arretes[i];
        arrete_t *current = liste->head;

        while (current != NULL) {
            int class_from = class_map[current->sommet_depart];
            int class_to = class_map[current->sommet_arrive];

            if (class_from != class_to) {
                fprintf(file, "%s-->%s\n", partition->classes[class_from].nom,
                        partition->classes[class_to].nom);
            }
            current = current->sommet_suivante;
        }
    }

    free(class_map);
    fclose(file);
    printf("Fichier Hasse Mermaid généré: %s\n", filename);
}

void analyze_graph_characteristics(const t_partition *partition, const liste_adjacence *graph) {
    if (partition == NULL || graph == NULL) return;

    printf("\n=== CARACTERISTIQUES DU GRAPHE ===\n");

    // Créer une map sommet -> classe
    int *class_map = (int *)malloc((graph->taille + 1) * sizeof(int));
    int *has_outgoing = (int *)calloc(partition->taille, sizeof(int));

    for (int i = 0; i < partition->taille; i++) {
        for (int j = 0; j < partition->classes[i].taille; j++) {
            class_map[partition->classes[i].sommets[j]] = i;
        }
    }

    // Vérifier les liens sortants entre classes
    for (int i = 0; i < graph->taille; i++) {
        list_t *liste = &graph->liste_arretes[i];
        arrete_t *current = liste->head;

        while (current != NULL) {
            int class_from = class_map[current->sommet_depart];
            int class_to = class_map[current->sommet_arrive];

            if (class_from != class_to) {
                has_outgoing[class_from] = 1;
            }
            current = current->sommet_suivante;
        }
    }

    // Analyser chaque classe
    int irreducible = (partition->taille == 1);

    for (int i = 0; i < partition->taille; i++) {
        if (has_outgoing[i]) {
            printf("Classe %s: TRANSTOIRE - ", partition->classes[i].nom);
            printf("Les états ");
            for (int j = 0; j < partition->classes[i].taille; j++) {
                printf("%d", partition->classes[i].sommets[j]);
                if (j < partition->classes[i].taille - 1) printf(",");
            }
            printf(" sont transitoires\n");
        } else {
            printf("Classe %s: PERSISTANTE - ", partition->classes[i].nom);
            printf("Les états ");
            for (int j = 0; j < partition->classes[i].taille; j++) {
                printf("%d", partition->classes[i].sommets[j]);
                if (j < partition->classes[i].taille - 1) printf(",");
            }
            printf(" sont persistants\n");

            // Vérifier les états absorbants
            if (partition->classes[i].taille == 1) {
                printf("  -> L'état %d est ABSORBANT\n", partition->classes[i].sommets[0]);
            }
        }
    }

    if (irreducible) {
        printf("Le graphe de Markov est IRREDUCTIBLE\n");
    } else {
        printf("Le graphe de Markov n'est pas irréductible\n");
    }

    free(class_map);
    free(has_outgoing);
}
