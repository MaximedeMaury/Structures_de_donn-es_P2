#include <stdio.h>
#include "list.h"
#include "utils.h"
#include "markov.h"
#include "tarjan.h"
#include "Matrix.h"

void verify_graph_edges(const liste_adjacence *graph) {
	printf("\n=== VERIFICATION DES ARETES ===\n");
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
			printf("AUCUNE ARETE");
		}
		printf("\n");
	}
	printf("Total des aretes: %d\n", total_edges);
}

void debug_graph_structure(const liste_adjacence *graph) {
	printf("=== STRUCTURE DU GRAPHE (Debug) ===\n");
	printf("Nombre de sommets: %d\n", graph->taille);

	for (int i = 0; i < graph->taille; i++) {
		list_t *liste = &graph->liste_arretes[i];
		printf("Sommet %d: ", i + 1);

		if (liste->head == NULL) {
			printf("AUCUNE ARETE\n");
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

const char *file_name = "data/exemple1_from_chatGPT.txt";

int main() {

	printf("=== CHARGEMENT DU GRAPHE ===\n");
	liste_adjacence *graphe = readGraph(file_name);

	printf("\n=== VERIFICATION DES ARETES ===\n");
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
	printf("\n=== ETAPE 3 : CALCULS MATRICIELS ===\n");
	t_matrix M = createMatrixFromGraph(graphe);
	printf("Matrice de transition M :\n");
	printMatrix(M);

	// 1. Calcul de M^3 (Prévision à 3 jours) [cite: 91]
	t_matrix M_pow = createEmptyMatrix(M.rows, M.cols);
	t_matrix M_res = createEmptyMatrix(M.rows, M.cols);
	copyMatrix(M_pow, M); // M^1

	// M^2
	multiplyMatrices(M_pow, M, M_res);
	copyMatrix(M_pow, M_res);
	// M^3
	multiplyMatrices(M_pow, M, M_res);
	copyMatrix(M_pow, M_res);

	printf("\nMatrice M^3 (Probabilites a 3 etapes) :\n");
	printMatrix(M_pow);

	// 2. Recherche de convergence (Stationnaire) [cite: 134]
	printf("\nRecherche de distribution stationnaire (Convergence)...\n");
	t_matrix M_prev = createEmptyMatrix(M.rows, M.cols);
	copyMatrix(M_pow, M); // Reset à M

	int k = 1;
	float diff = 1.0f;
	while (diff > 0.001f && k < 1000) { // Limite de sécurité
		copyMatrix(M_prev, M_pow);
		multiplyMatrices(M_prev, M, M_pow); // M^(k+1)
		diff = diffMatrix(M_pow, M_prev);
		k++;
	}
	printf("Convergence atteinte a k=%d (diff=%.5f)\n", k, diff);
	printf("Distribution stationnaire approximative :\n");
	printMatrix(M_pow);

	// 3. Bonus : Périodicité par classe
	printf("\n=== BONUS : PERIODICITE ===\n");
	for (int i = 0; i < partition->taille; i++) {
		t_matrix sub = subMatrix(M, partition, i);
		int p = getPeriod(sub);
		printf("Classe %s : Periode = %d\n", partition->classes[i].nom, p);
		freeMatrix(&sub);
	}

	// Nettoyage matrices
	freeMatrix(&M);
	freeMatrix(&M_pow);
	freeMatrix(&M_res);
	freeMatrix(&M_prev);

	printf("\n=== NETTOYAGE MEMOIRE ===\n");
	free_partition(partition);
	graph_free(graphe);

	return 0;
}