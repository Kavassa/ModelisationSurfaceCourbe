#include "o_objet.h"
#include "t_geometrie.h"
#include <math.h>
//#include "sys_lin.c"

Grille_flottant allocationMatrice( int ligne, int colonne);
Grille_flottant multiplication_matrice_matrice(Grille_flottant* matrice_1, Grille_flottant* matrice_2);
Table_flottant multiplication_matrice_vecteur(Grille_flottant* g, Table_flottant* f);

Grille_flottant transposition_matrice(Grille_flottant* matrice);
int factorielle( int n);
double bernstein( int i,  int degre, double u);
Grille_flottant creerMatriceCoeffBernstein( int degre, Booleen uniforme, Table_quadruplet tab);
Table_quadruplet calcul(int degre, Table_quadruplet tab, Booleen uniforme);

double calculDistance(Quadruplet* q1, Quadruplet* q2);

void affichageGrilleFlottant(Grille_flottant* g);
void affichageTableFlottant(Table_flottant* tab);
void affichageTableQuadruplet(Table_quadruplet* tab);

Table_flottant getCoordX(Table_quadruplet* tab);
Table_flottant getCoordY(Table_quadruplet* tab);
Table_flottant getCoordZ(Table_quadruplet* tab);

void setCoordX(Table_quadruplet* tab_quad, Table_flottant* tab_float);
void setCoordY(Table_quadruplet* tab_quad, Table_flottant* tab_float);
void setCoordZ(Table_quadruplet* tab_quad, Table_flottant* tab_float);
void setCoordHa1(Table_quadruplet* tab_quad);

Grille_flottant duplique_Grille_flottant(Grille_flottant *original);
Table_flottant duplique_Table_flottant(Table_flottant *original);
void traite_pivot_max(Grille_flottant  *A,Table_flottant  *f, int colonne);
int resolution_systeme_lineaire(Grille_flottant  *A, 
                                Table_flottant  *f, 
                                Table_flottant  *x);