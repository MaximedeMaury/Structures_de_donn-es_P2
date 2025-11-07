#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"

t_tarjan_vertex *init_tarjan_vertices(const liste_adjacence *graph) {
    if (graph == NULL) return NULL;

    t_tarjan_vertex *vertices = (t_tarjan_vertex *)malloc(graph->taille * sizeof(t_tarjan_vertex));
    if (vertices == NULL) {
        perror("malloc(vertices)");
        return NULL;
    }

    for (int i = 0; i < graph->taille; i++) {
        vertices[i].id = i + 1;  // Les sommets commencent à 1
        vertices[i].index = -1;
        vertices[i].lowlink = -1;
        vertices[i].on_stack = 0;
    }

    return vertices;
}

t_stack *create_stack(int capacite) {
    t_stack *stack = (t_stack *)malloc(sizeof(t_stack));
    if (stack == NULL) {
        perror("malloc(stack)");
        return NULL;
    }

    stack->elements = (int *)malloc(capacite * sizeof(int));
    if (stack->elements == NULL) {
        free(stack);
        perror("malloc(stack->elements)");
        return NULL;
    }

    stack->taille = 0;
    stack->capacite = capacite;
    return stack;
}

void stack_push(t_stack *stack, int element) {
    if (stack->taille >= stack->capacite) {
        // Redimensionner
        stack->capacite *= 2;
        stack->elements = (int *)realloc(stack->elements, stack->capacite * sizeof(int));
    }
    stack->elements[stack->taille++] = element;
}

int stack_pop(t_stack *stack) {
    if (stack->taille == 0) return -1;
    return stack->elements[--stack->taille];
}

int stack_is_empty(const t_stack *stack) {
    return stack->taille == 0;
}

t_partition *create_partition() {
    t_partition *partition = (t_partition *)malloc(sizeof(t_partition));
    if (partition == NULL) {
        perror("malloc(partition)");
        return NULL;
    }

    partition->capacite = 10;
    partition->taille = 0;
    partition->classes = (t_classe *)malloc(partition->capacite * sizeof(t_classe));
    if (partition->classes == NULL) {
        free(partition);
        perror("malloc(partition->classes)");
        return NULL;
    }

    return partition;
}

void partition_add_class(t_partition *partition, t_classe *classe) {
    if (partition->taille >= partition->capacite) {
        partition->capacite *= 2;
        partition->classes = (t_classe *)realloc(partition->classes,
                                                 partition->capacite * sizeof(t_classe));
    }
    partition->classes[partition->taille++] = *classe;
}

t_classe *create_classe(const char *nom) {
    t_classe *classe = (t_classe *)malloc(sizeof(t_classe));
    if (classe == NULL) {
        perror("malloc(classe)");
        return NULL;
    }

    strncpy(classe->nom, nom, 9);
    classe->nom[9] = '\0';
    classe->taille = 0;
    classe->capacite = 10;
    classe->sommets = (int *)malloc(classe->capacite * sizeof(int));
    if (classe->sommets == NULL) {
        free(classe);
        perror("malloc(classe->sommets)");
        return NULL;
    }

    return classe;
}

void classe_add_sommet(t_classe *classe, int sommet) {
    if (classe->taille >= classe->capacite) {
        classe->capacite *= 2;
        classe->sommets = (int *)realloc(classe->sommets, classe->capacite * sizeof(int));
    }
    classe->sommets[classe->taille++] = sommet;
}

void free_partition(t_partition *partition) {
    if (partition == NULL) return;

    for (int i = 0; i < partition->taille; i++) {
        free(partition->classes[i].sommets);
    }
    free(partition->classes);
    free(partition);
}

void free_stack(t_stack *stack) {
    if (stack == NULL) return;
    free(stack->elements);
    free(stack);
}

void strongconnect(t_tarjan_vertex *vertices, int v, t_stack *stack, int *index,
                   const liste_adjacence *graph, t_partition *partition) {
    vertices[v].index = *index;
    vertices[v].lowlink = *index;
    (*index)++;
    stack_push(stack, v);
    vertices[v].on_stack = 1;

    // Parcourir tous les voisins
    list_t *liste = &graph->liste_arretes[v];
    arrete_t *current = liste->head;
    while (current != NULL) {
        int w = current->sommet_arrive - 1;  // Convertir en index 0-based

        if (vertices[w].index == -1) {
            // Sommet non visité
            strongconnect(vertices, w, stack, index, graph, partition);
            if (vertices[w].lowlink < vertices[v].lowlink) {
                vertices[v].lowlink = vertices[w].lowlink;
            }
        } else if (vertices[w].on_stack) {
            // Sommet dans la pile
            if (vertices[w].index < vertices[v].lowlink) {
                vertices[v].lowlink = vertices[w].index;
            }
        }
        current = current->sommet_suivante;
    }

    // Si v est une racine, créer une nouvelle composante
    if (vertices[v].lowlink == vertices[v].index) {
        static int class_count = 1;
        char nom[10];
        snprintf(nom, sizeof(nom), "C%d", class_count++);
        t_classe *classe = create_classe(nom);

        int w;
        do {
            w = stack_pop(stack);
            vertices[w].on_stack = 0;
            classe_add_sommet(classe, w + 1);  // Convertir en numéro 1-based
        } while (w != v);

        partition_add_class(partition, classe);
        free(classe);  // La classe est copiée dans la partition
    }
}

t_partition *tarjan(const liste_adjacence *graph) {
    if (graph == NULL) return NULL;

    t_tarjan_vertex *vertices = init_tarjan_vertices(graph);
    t_stack *stack = create_stack(graph->taille);
    t_partition *partition = create_partition();
    int index = 0;

    for (int v = 0; v < graph->taille; v++) {
        if (vertices[v].index == -1) {
            strongconnect(vertices, v, stack, &index, graph, partition);
        }
    }

    free(vertices);
    free_stack(stack);
    return partition;
}

void print_partition(const t_partition *partition) {
    if (partition == NULL) return;

    for (int i = 0; i < partition->taille; i++) {
        printf("Composante %s: {", partition->classes[i].nom);
        for (int j = 0; j < partition->classes[i].taille; j++) {
            printf("%d", partition->classes[i].sommets[j]);
            if (j < partition->classes[i].taille - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }
}
