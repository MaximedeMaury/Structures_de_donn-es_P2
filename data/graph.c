#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


char* getID(int i);

Cellule* creerCellule(int arrivee, float proba) {
    Cellule* newCell = (Cellule*)malloc(sizeof(Cellule));
    if (!newCell) {
        perror("Allocation de cellule échouée");
        exit(EXIT_FAILURE);
    }
    newCell->sommet_arrivee = arrivee;
    newCell->probabilite = proba;
    newCell->suiv = NULL;
    return newCell;
}

Liste* creerListe() {
    Liste* l = (Liste*)malloc(sizeof(Liste));
    if (!l) {
        perror("Allocation de liste échouée");
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    return l;
}

void ajouterEnTete(Liste* l, int arrivee, float proba) {
    Cellule* newCell = creerCellule(arrivee, proba);
    newCell->suiv = l->head;
    l->head = newCell;
}

void afficherListe(Liste* l) {
    Cellule* courant = l->head;
    while (courant != NULL) {
        printf(" -> (%d, %.2f)", courant->sommet_arrivee, courant->probabilite);
        courant = courant->suiv;
    }
    printf("\n");
}

ListeAdjacence* creerListeAdjacence(int taille) {
    ListeAdjacence* adj = (ListeAdjacence*)malloc(sizeof(ListeAdjacence));
    if (!adj) {
        perror("Allocation de la liste d'adjacence échouée");
        exit(EXIT_FAILURE);
    }
    adj->nb_sommets = taille;
    adj->tab = (Liste*)malloc(taille * sizeof(Liste));
    if (!adj->tab) {
        perror("Allocation du tableau de listes échouée");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < taille; i++) {
        adj->tab[i].head = NULL;
    }
    return adj;
}

void afficherListeAdjacence(ListeAdjacence* adj) {
    if (!adj) return;
    printf("Affichage de la liste d'adjacence (%d sommets) :\n", adj->nb_sommets);
    for (int i = 0; i < adj->nb_sommets; i++) {
        printf("Sommet %d :", i + 1);
        afficherListe(&(adj->tab[i]));
    }
}

ListeAdjacence* readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier en lecture");
        exit(EXIT_FAILURE);
    }

    int nbvert, depart, arrivee;
    float proba;

    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Impossible de lire le nombre de sommets");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    ListeAdjacence* adj = creerListeAdjacence(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart > 0 && depart <= adj->nb_sommets) {
            ajouterEnTete(&(adj->tab[depart - 1]), arrivee, proba);
        }
    }

    fclose(file);
    return adj;
}

void libererGraphe(ListeAdjacence* adj) {
    if (!adj) return;
    for (int i = 0; i < adj->nb_sommets; i++) {
        Cellule* courant = adj->tab[i].head;
        while (courant != NULL) {
            Cellule* aSupprimer = courant;
            courant = courant->suiv;
            free(aSupprimer);
        }
    }
    free(adj->tab);
    free(adj);
}

void verifierGrapheMarkov(ListeAdjacence* adj) {
    if (!adj) return;
    printf("\n--- Vérification du graphe de Markov ---\n");
    int estMarkov = 1;

    for (int i = 0; i < adj->nb_sommets; i++) {
        float sommeProbas = 0.0f;
        Cellule* courant = adj->tab[i].head;
        while (courant != NULL) {
            sommeProbas += courant->probabilite;
            courant = courant->suiv;
        }

        if (sommeProbas < 0.99f || sommeProbas > 1.01f) {
                printf("Le graphe n'est pas un graphe de Markov\n");
            }
            printf(" -> la somme des probabilités du sommet %d est %.4f\n", i + 1, sommeProbas);
            estMarkov = 0;
        }
    }

    if (estMarkov) {
        printf("Le graphe est un graphe de Markov\n");
    }
}



void dessinerGrapheMermaid(ListeAdjacence* adj, const char* output_filename) {
    if (!adj) return;
    FILE* file = fopen(output_filename, "wt");
    if (file == NULL) {
        perror("Impossible d'ouvrir le fichier en écriture pour Mermaid");
        return;
    }

    printf("\n--- Génération du fichier %s pour Mermaid ---\n", output_filename);


    fprintf(file, "config:\n");
    fprintf(file, "  layout: elk\n");
    fprintf(file, "  theme: neo\n");
    fprintf(file, "  look: neo\n");
    fprintf(file, "flowchart LR\n");
    for (int i = 1; i <= adj->nb_sommets; i++) {
        fprintf(file, "  %s((%d))\n", getID(i), i);
    }

    for (int i = 0; i < adj->nb_sommets; i++) {
        int sommet_depart = i + 1;
        Cellule* courant = adj->tab[i].head;
        while (courant != NULL) {
            fprintf(file, "  %s-->|%.2f|%s\n",
                    getID(sommet_depart),
                    courant->probabilite,
                    getID(courant->sommet_arrivee));
            courant = courant->suiv;
        }
    }

    fclose(file);
    printf("Fichier %s généré avec succès.\n", output_filename);
    printf("Copiez/collez son contenu dans l'éditeur Mermaid : https://mermaid.live/\n");
}