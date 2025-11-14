#include <stdio.h>
#include "list.h"
#include "utils.h"
#include "markov.h"
#include "tarjan.h"

void verify_graph_edges(const liste_adjacence *graph) {
	printf("\n=== VERIFICATION DES ARÊTES ===\n");
	int total_edges = 0;

	for (int i = 0; i < graph->taille; i++) {
		list_t *liste = &graph->liste_arretes[i];
		arrete_t *current = liste->head;
		int edge_count = 0;

		printf("Sommet %d: ", i + 1);
		while (current != NULL) {
			printf("%d->%d(%.3f) ", current->sommet_depart, current->sommet_arrive, current->probabilite);
			current = current->sommet_suivante;
			edge_count++;
			total_edges++;
		}

		if (edge_count == 0) {
			printf("AUCUNE ARÊTE");
		}
		printf("\n");
	}
	printf("Total des arêtes: %d\n", total_edges);
}

void debug_graph_structure(const liste_adjacence *graph) {
	printf("=== STRUCTURE DU GRAPHE (Debug) ===\n");
	printf("Nombre de sommets: %d\n", graph->taille);

	for (int i = 0; i < graph->taille; i++) {
		list_t *liste = &graph->liste_arretes[i];
		printf("Sommet %d: ", i + 1);

		if (liste->head == NULL) {
			printf("AUCUNE ARÊTE\n");
		} else {
			arrete_t *current = liste->head;
			while (current != NULL) {
				printf("%d->%d(%.3f) ", current->sommet_depart, current->sommet_arrive, current->probabilite);
				current = current->sommet_suivante;
			}
			printf("\n");
		}
	}
	printf("=== FIN DEBUG ===\n");
}

void debug_mermaid_output(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		printf("Impossible d'ouvrir le fichier %s\n", filename);
		return;
	}

	printf("=== CONTENU DU FICHIER MERMAID ===\n");
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		printf("%s", line);
	}
	fclose(file);
	printf("=== FIN DU CONTENU ===\n");
}

const char *file_name = "data/exemple1.txt";

int main() {

	printf("=== CHARGEMENT DU GRAPHE ===\n");
	liste_adjacence *graphe = readGraph(file_name);

	printf("\n=== VERIFICATION DES ARÊTES ===\n");
	verify_graph_edges(graphe);

	printf("\n=== AFFICHAGE DU GRAPHE ===\n");
	graph_print(graphe);

	printf("\n=== GENERATION GRAPHE MERMAID ===\n");
	generate_mermaid_graph(graphe, "graphe_mermaid.mmd");
	debug_mermaid_output("graphe_mermaid.mmd");

	printf("\n=== AFFICHAGE DU GRAPHE ===\n");
	graph_print(graphe);

	printf("\n=== VERIFICATION MARKOV ===\n");
	verify_markov_property(graphe);

	printf("\n=== GENERATION GRAPHE MERMAID ===\n");
	generate_mermaid_graph(graphe, "graphe_mermaid.mmd");

	printf("\n=== ALGORITHME DE TARJAN ===\n");
	t_partition *partition = tarjan(graphe);
	print_partition(partition);

	printf("\n=== GENERATION DIAGRAMME DE HASSE ===\n");
	generate_mermaid_hasse(partition, graphe, "hasse_mermaid.mmd");

	printf("\n=== ANALYSE DES CARACTERISTIQUES ===\n");
	analyze_graph_characteristics(partition, graphe);

	printf("\n=== NETTOYAGE MEMOIRE ===\n");
	free_partition(partition);
	graph_free(graphe);

	return 0;
}