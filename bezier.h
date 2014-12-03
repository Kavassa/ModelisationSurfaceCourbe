#include "o_objet.h"
#include "t_geometrie.h"
#include <math.h>

/*	Prend un quadruplet en entree
	Divise ses coordonnées par son poids (4ieme coord)
	Retourne un triplet */
Triplet divisionParPoidsQuadTripl(Quadruplet point);

/*	Prend un quadruplet en entree
	Divise toutes ses coordonnées par son poids (4ieme coord)
	Retourne un quadruplet */
Quadruplet divisionParPoidsQuadQuad(Quadruplet point);

/*	Prend un quadruplet en entree
	Multiplie toutes ses coordonnées par son poids (4ieme coord)
	Retourne un quadruplet */
Quadruplet multiplicationParPoidsQuadQuad(Quadruplet point);

/*	Applique la formule de casteljau
	Cree un nouveau point à partir de deux autres*/
Quadruplet applicationFormuleCasteljau(Quadruplet point_plus_un, Quadruplet point, double u);

/*	Retourne une courbe de bezier */
Table_quadruplet calcul_bezier(Table_quadruplet tabPointControle, int nbPointAffiche);

/*	Retourne les points de controle d'une courbe de bezier sur un interval */
Table_quadruplet calculPointsCourbeInterval(Table_quadruplet tab_point_controle, double interval_min, double interval_max);

/*	Retourne les points de controle de la partie gauche d'une courbe de bezier */
Table_quadruplet calculPointsCourbeGauche(Table_quadruplet tab_point_controle, double interval_min, double interval_max);
