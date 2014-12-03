#include "o_objet.h"
#include "t_geometrie.h"
#include <math.h>
#include "bezier.h"



Table_flottant creationVecteurNodal( int degre, int nb_point_controle);
int retourneIndiceVectreurNodale(Table_flottant tab, int degre, int nbPointControle, double u);
Table_quadruplet calcul_bsplines(Table_quadruplet tab_point_controle, int nb_points_a_afficher, Table_flottant tab, int degre);