#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_fichier_graphe>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* input_filename = argv[1];


    printf("--- Chargement du graphe depuis %s ---\n", input_filename);
    ListeAdjacence* monGraphe = readGraph(input_filename);


    afficherListeAdjacence(monGraphe);


    verifierGrapheMarkov(monGraphe);


    char output_filename[100];
    sprintf(output_filename, "mermaid_output.txt");
    dessinerGrapheMermaid(monGraphe, output_filename);


    libererGraphe(monGraphe);

    return EXIT_SUCCESS;
}