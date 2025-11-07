#ifndef INC_2526_TI301_PJT_MARKOV_H
#define INC_2526_TI301_PJT_MARKOV_H

#include "list.h"
#include "tarjan.h"

// Vérification du graphe de Markov
int is_markov_graph(const liste_adjacence *graph);
void verify_markov_property(const liste_adjacence *graph);

// Génération Mermaid
void generate_mermaid_graph(const liste_adjacence *graph, const char *filename);
void generate_mermaid_hasse(const t_partition *partition, const liste_adjacence *graph, const char *filename);

// Analyse des caractéristiques
void analyze_graph_characteristics(const t_partition *partition, const liste_adjacence *graph);


#endif //INC_2526_TI301_PJT_MARKOV_H
