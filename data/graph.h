#ifndef GRAPH_H
#define GRAPH_H
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct Cellule {
    int sommet_arrivee;
    float probabilite;
    struct Cellule* suiv;
} Cellule;


typedef struct Liste {
    Cellule* head;
} Liste;

typedef struct ListeAdjacence {
    int nb_sommets;
    Liste* tab;
} ListeAdjacence;

Cellule* creerCellule(int arrivee, float proba);
Liste* creerListe();
void ajouterEnTete(Liste* l, int arrivee, float proba);
void afficherListe(Liste* l);
ListeAdjacence* creerListeAdjacence(int taille);
void afficherListeAdjacence(ListeAdjacence* adj);
ListeAdjacence* readGraph(const char* filename);
void libererGraphe(ListeAdjacence* adj);


void verifierGrapheMarkov(ListeAdjacence* adj);

void dessinerGrapheMermaid(ListeAdjacence* adj, const char* output_filename);

#endif // GRAPH_H
#endif //GRAPH_H
