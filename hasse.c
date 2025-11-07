#include <malloc.h>
#include "hasse.h"
#include "tarjan.h"
#include "list.h"

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

t_link_array *create_link_array(const t_partition *part, const liste_adjacence *graph) {
    if (part == NULL || graph == NULL) return NULL;

    t_link_array *link_array = (t_link_array *)malloc(sizeof(t_link_array));
    if (link_array == NULL) return NULL;

    link_array->phys_size = 100;
    link_array->log_size = 0;
    link_array->links = (t_link *)malloc(link_array->phys_size * sizeof(t_link));

    // Créer une map sommet -> classe
    int *class_map = (int *)malloc((graph->taille + 1) * sizeof(int));
    for (int i = 0; i < part->taille; i++) {
        for (int j = 0; j < part->classes[i].taille; j++) {
            class_map[part->classes[i].sommets[j]] = i;
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
                // Vérifier si le lien existe déjà
                int exists = 0;
                for (int k = 0; k < link_array->log_size; k++) {
                    if (link_array->links[k].from == class_from &&
                        link_array->links[k].to == class_to) {
                        exists = 1;
                        break;
                    }
                }

                if (!exists) {
                    if (link_array->log_size >= link_array->phys_size) {
                        link_array->phys_size *= 2;
                        link_array->links = (t_link *)realloc(link_array->links,
                                                              link_array->phys_size * sizeof(t_link));
                    }
                    link_array->links[link_array->log_size].from = class_from;
                    link_array->links[link_array->log_size].to = class_to;
                    link_array->log_size++;
                }
            }
            current = current->sommet_suivante;
        }
    }

    free(class_map);
    return link_array;
}

void free_link_array(t_link_array *link_array) {
    if (link_array == NULL) return;
    free(link_array->links);
    free(link_array);
}
