#include "list.h"

#include <stdio.h>
#include <stdlib.h>

//arrete_t *create_cell(int sommet_depart, int sommet_arrive, float probabilite) {
//    arrete_t *cellule;
//    cellule = (arrete_t *) malloc(sizeof(arrete_t));
//    if (NULL == cellule) {
//        perror("malloc(cellule)");
//        return NULL;
//    }
//
//    cellule->sommet_depart = sommet_depart;
//    cellule->sommet_arrive = sommet_arrive;
//    cellule->probabilite = probabilite;
//    cellule->sommet_suivante = NULL;
//
//    return cellule;
//}

arrete_t *create_cell(int sommet_depart, int sommet_arrive, float probabilite) {
    arrete_t *cellule;
    cellule = (arrete_t *) malloc(sizeof(arrete_t));
    if (NULL == cellule) {
        perror("malloc(cellule)");
        return NULL;
    }

    cellule->sommet_depart = sommet_depart;
    cellule->sommet_arrive = sommet_arrive;  // Vérifier que c'est bien arrivé
    cellule->probabilite = probabilite;
    cellule->sommet_suivante = NULL;

    printf("DEBUG create_cell: Création cellule %d -> %d (prob=%.3f)\n",
           sommet_depart, sommet_arrive, probabilite);

    return cellule;
}

list_t *create_list() {
    list_t *list;
    list = (list_t *) malloc(sizeof(list_t));
    if (NULL == list) {
        perror("malloc(list)");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;

    return list;
}

//void list_add_cell(list_t *list, int sommet_depart, int sommet_arrive, float probabilite) {
//    if (list == NULL) return;
//
//    arrete_t *new_cell = create_cell(sommet_depart, sommet_arrive, probabilite);
//    if (new_cell == NULL) return;
//
//    new_cell->sommet_suivante = NULL;
//
//    // si la liste est vide.
//    if (list->head == NULL) {
//        list->head = new_cell;
//        list->tail = new_cell;
//    } else {
//        list->tail->sommet_suivante = new_cell;
//        list->tail = new_cell;
//    }
//}

void list_add_cell(list_t *list, int sommet_depart, int sommet_arrive, float probabilite) {
    if (list == NULL) return;

    arrete_t *new_cell = create_cell(sommet_depart, sommet_arrive, probabilite);
    if (new_cell == NULL) return;

    // Debug important
    printf("DEBUG list_add_cell: Ajout arete %d -> %d (prob=%.3f)\n",
           sommet_depart, sommet_arrive, probabilite);

    // si la liste est vide.
    if (list->head == NULL) {
        list->head = new_cell;
        list->tail = new_cell;
    } else {
        list->tail->sommet_suivante = new_cell;
        list->tail = new_cell;
    }
}

void list_print(const list_t *list) {
    if (list == NULL) return;

    arrete_t *current = list->head;
    printf("Liste pour le sommet %d:[head @] ", current->sommet_depart);
    while (current != NULL) {
        printf("-> (%d, %.3f) ", current->sommet_arrive, current->probabilite);
        current = current->sommet_suivante;
    }
}

void list_destroy(list_t *list) {
    if (list == NULL) return;

    arrete_t *current = list->head;
    while (current != NULL) {
        arrete_t *next = current->sommet_suivante;
        free(current);
        current = next;
    }

    // Réinitialiser la liste
    list->head = NULL;
    list->tail = NULL;
}

liste_adjacence *create_graph(int taille) {
    liste_adjacence *graph = (liste_adjacence *) malloc(sizeof(liste_adjacence));
    if (graph == NULL) {
        perror("malloc(graphe)");
        return NULL;
    }

    graph->taille = taille;
    graph->liste_arretes = (list_t *) malloc(taille * sizeof(list_t));
    if (graph->liste_arretes == NULL) {
        perror("malloc(graphe.liste_arretes)");
        free(graph);
        return NULL;
    }

    // initialise la liste d'adjacence à vide
    for (int i = 0; i < taille; i++) {
        graph->liste_arretes[i].head = NULL;
        graph->liste_arretes[i].tail = NULL;
    }

    return graph;
}

void graph_print(const liste_adjacence *graph) {
    if (graph == NULL) return;

    for (int i = 0; i < graph->taille; i++) {
        list_print(&graph->liste_arretes[i]);
        printf("\n");
    }
}

void graph_free(liste_adjacence *graph) {
    if (graph == NULL) return;

    if (graph->liste_arretes != NULL) {
        for (int i = 0; i < graph->taille; ++i) {
            list_destroy(&graph->liste_arretes[i]);
        }
        free(graph->liste_arretes);
    }

    free(graph);
}
