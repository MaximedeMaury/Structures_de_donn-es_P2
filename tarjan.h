#ifndef INC_2526_TI301_PJT_TARJAN_H
#define INC_2526_TI301_PJT_TARJAN_H

#include "list.h"

typedef struct {
    int id;                    // Identifiant du sommet (numéro dans le graphe)
    int index;                 // Numéro de parcours
    int lowlink;               // Plus petit index accessible
    int on_stack;              // 1 si dans la pile, 0 sinon
} t_tarjan_vertex;

typedef struct {
    int *sommets;              // Tableau des sommets de la classe
    int taille;                // Nombre de sommets dans la classe
    int capacite;              // Capacité du tableau
    char nom[10];              // Nom de la classe (C1, C2, ...)
} t_classe;

typedef struct {
    t_classe *classes;         // Tableau des classes
    int taille;                // Nombre de classes
    int capacite;              // Capacité du tableau
} t_partition;

typedef struct {
    int *elements;             // Tableau des éléments de la pile
    int taille;                // Taille actuelle
    int capacite;              // Capacité maximale
} t_stack;

// Fonctions pour Tarjan
t_tarjan_vertex *init_tarjan_vertices(const liste_adjacence *graph);
t_stack *create_stack(int capacite);
void stack_push(t_stack *stack, int element);
int stack_pop(t_stack *stack);
int stack_is_empty(const t_stack *stack);
t_partition *create_partition();
void partition_add_class(t_partition *partition, t_classe *classe);
t_classe *create_classe(const char *nom);
void classe_add_sommet(t_classe *classe, int sommet);
void free_partition(t_partition *partition);
void free_stack(t_stack *stack);

// Algorithme de Tarjan
void strongconnect(t_tarjan_vertex *vertices, int v, t_stack *stack, int *index,
                   const liste_adjacence *graph, t_partition *partition);
t_partition *tarjan(const liste_adjacence *graph);

// Affichage
void print_partition(const t_partition *partition);

#endif
