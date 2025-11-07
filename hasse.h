#ifndef __HASSE_H__
#define __HASSE_H__

#include "list.h"
#include "tarjan.h"

typedef struct {
    int from;
    int to;
} t_link;

typedef struct {
    t_link *links;
    int log_size;
    int phys_size;
} t_link_array;

void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */
t_link_array *create_link_array(const t_partition *part, const liste_adjacence *graph);

void free_link_array(t_link_array *link_array);

void removeTransitiveLinks(t_link_array *p_link_array);

/**
 * @brief Creates a link array from the given partition and graph.
 *
 * @param part The partition of the graph.
 * @param graph The adjacency list representation of the graph.
 * @return The created link array.
 */

#endif