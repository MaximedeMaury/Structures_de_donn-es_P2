#ifndef INC_2526_TI301_PJT_LIST_H
#define INC_2526_TI301_PJT_LIST_H

typedef struct __cellule__t {
	int sommet_depart; // le sommet de depart de l'arête
	int sommet_arrive; // le sommet d'arrivée de cette arête
	float probabilite; // la probabilité de passage au sommet suivant
	struct __cellule__t *sommet_suivante; // le sommet suivant dans le graphe
} arrete_t;

typedef struct {
	arrete_t *head; // le premier sommet de la liste adjacente
	arrete_t *tail;
} list_t;

typedef struct {
	int taille; // taille du graphe
	list_t *liste_arretes; // tableau dynamique des listes adjacente
} liste_adjacence;

// une fonction pour créer une « cellule »
arrete_t *create_cell(int sommet_depart, int sommet_arrive, float probabilite);

// une fonction pour créer une « liste » vide ;
list_t *create_list();

// une fonction pour ajouter une cellule à une liste ;
void list_add_cell(list_t *list, /*arrete_t *cellule*/ int sommet_depart, int sommet_arrive, float probabilite);

// une fonction pour afficher une liste ;
void list_print(const list_t *list);

// une fonction pour détruire une liste ;
void list_destroy(list_t *list);

// une fonction pour créer une liste d’adjacence ‘vide’ à partir d’une taille donnée – on crée le tableau de listes, chacune étant initialisée avec la liste vide ;
liste_adjacence *create_graph(int taille);

// Une fonction pour aƯicher une liste d’adjacence.
void graph_print(const liste_adjacence *graph);

// Une fonction pour détruire la liste adjacente
void graph_free(liste_adjacence *graph);

#endif //INC_2526_TI301_PJT_LIST_H
