#include "bezier.h"

Triplet divisionParPoidsQuadTripl(Quadruplet point)
{
	Triplet pt_res;
	pt_res.x = point.x / point.h;
	pt_res.y = point.y / point.h;
	pt_res.z = point.z / point.h;
	return pt_res;
}

Quadruplet divisionParPoidsQuadQuad(Quadruplet point)
{
	Quadruplet pt_res;
	pt_res.x = point.x / point.h;
	pt_res.y = point.y / point.h;
	pt_res.z = point.z / point.h;
	pt_res.h = point.h;
	return pt_res;
}

Quadruplet multiplicationParPoidsQuadQuad(Quadruplet point)
{
	Quadruplet pt_res;
	pt_res.x = point.x * point.h;
	pt_res.y = point.y * point.h;
	pt_res.z = point.z * point.h;
	pt_res.h = point.h;
	return pt_res;
}

Quadruplet applicationFormuleCasteljau(Quadruplet point_plus_un, Quadruplet point, double u)
{
	Quadruplet pt_res;
	pt_res.x = u * point_plus_un.x + (1-u) * point.x;	
	pt_res.y = u * point_plus_un.y + (1-u) * point.y;
	pt_res.z = u * point_plus_un.z + (1-u) * point.z;
	pt_res.h = u * point_plus_un.h + (1-u) * point.h;
	return pt_res;
}






Table_quadruplet calcul_bezier(Table_quadruplet tab_point_controle, int nb_points_a_afficher)
{          
	int i;     
	int j;
	int h;
	int l;

	//-1 car les deux extremités ne sont pas calculés
	double u = (double)1/(nb_points_a_afficher-1);

	//Tableau des points resultats représentant la courbe
	Table_quadruplet tab_quadruplet_courbe;
	tab_quadruplet_courbe.nb = 0;
	//Tableau que l'on va utiliser pour stocker les calculs
	Table_quadruplet tab_quad_calcul;
	//Allocation des deux tableaux
	ALLOUER(tab_quadruplet_courbe.table, nb_points_a_afficher);
	ALLOUER(tab_quad_calcul.table, tab_point_controle.nb);
	
	//tab_quadruplet_courbe.table[0] = divisionParPoidsQuadQuad(tab_point_controle.table[0]);
	tab_quadruplet_courbe.table[0] = tab_point_controle.table[0];
	tab_quadruplet_courbe.nb++;
	//1 et -1 car les points de'extremités sont les points de controle
	for(h = 1; h < nb_points_a_afficher-1; h++)
	{	
		//On stocke les points de controle dans le tableau de calcul
		for(j = 0; j < tab_point_controle.nb; j++)
		{
			tab_quad_calcul.table[j] = tab_point_controle.table[j];
			tab_quad_calcul.table[j] = multiplicationParPoidsQuadQuad(tab_quad_calcul.table[j]);
		}
		//On met a jour le nombre d'éléments dans le tableau
		tab_quad_calcul.nb = tab_point_controle.nb;

		//On applique les formules du cour
		for(l = tab_quad_calcul.nb-1; l > 0; l--)
		{
			for(i = 0; i < l; i++)		
			{
				//Calcul
				tab_quad_calcul.table[i] = applicationFormuleCasteljau(tab_quad_calcul.table[i+1], tab_quad_calcul.table[i], u);
			}
		}

		//On stocke le nouveau point créé
		//tab_quadruplet_courbe.table[h] = divisionParPoidsQuadQuad(tab_quad_calcul.table[0]);
		tab_quadruplet_courbe.table[h] = tab_quad_calcul.table[0];
		tab_quadruplet_courbe.nb++;

		u += (double)1/(nb_points_a_afficher-1);
	}
	//tab_quadruplet_courbe.table[nb_points_a_afficher-1] = divisionParPoidsQuadQuad(tab_point_controle.table[tab_point_controle.nb-1]);
	tab_quadruplet_courbe.table[nb_points_a_afficher-1] = tab_point_controle.table[tab_point_controle.nb-1];
	tab_quadruplet_courbe.nb++;

	//Libération du tableau
    free(tab_quad_calcul.table);
	
	return tab_quadruplet_courbe;
}






Table_quadruplet calculPointsCourbeInterval(Table_quadruplet tab_point_controle, double interval_min, double interval_max)
{          
	int i;     
	int j;
	int l;

	double u = interval_min;
	
	Table_quadruplet tab_points_courbe_interval;
	Table_quadruplet tab_quad_calcul;

	Table_quadruplet tab_points_controle_droite;
	tab_points_controle_droite.nb = 0;

	ALLOUER(tab_points_controle_droite.table, tab_point_controle.nb);
	ALLOUER(tab_quad_calcul.table, tab_point_controle.nb);
		
	//Place les points de controle dans le tableau de calcul
	for(j = 0; j < tab_point_controle.nb; j++)
	{
		tab_quad_calcul.table[j]= tab_point_controle.table[j];
		tab_quad_calcul.table[j] = multiplicationParPoidsQuadQuad(tab_quad_calcul.table[j]);
	}

	//Applique casteljau pour le point du u=interval_min
	for(l = tab_point_controle.nb-1; l > 0; l--)
	{
		for(i = 0; i < l; i++)		
		{
			//Calcul
			tab_quad_calcul.table[i] = applicationFormuleCasteljau(tab_quad_calcul.table[i+1], tab_quad_calcul.table[i], u);
		}
	}

	//Recupere les coordonnées des points
	for(j = 0; j < tab_point_controle.nb; j++)
	{
		tab_points_controle_droite.table[j] = tab_quad_calcul.table[j];
		tab_points_controle_droite.table[j] = divisionParPoidsQuadQuad(tab_points_controle_droite.table[j]);
	}
	tab_points_controle_droite.nb = tab_point_controle.nb;

	//On récupère la courbe de la partie gauche des nouveaux points de controle
	tab_points_courbe_interval = calculPointsCourbeGauche(tab_points_controle_droite, interval_min, interval_max);

    free(tab_quad_calcul.table);
    free(tab_points_controle_droite.table);

	return tab_points_courbe_interval;
}






Table_quadruplet calculPointsCourbeGauche(Table_quadruplet tab_point_controle, double interval_min, double interval_max)
{          
	int i;  
	int l;

	double u = (interval_max - interval_min)/(1-interval_min);

	Table_quadruplet tab_quad_calcul;
	Table_quadruplet tab_points_controle_gauche;
	tab_points_controle_gauche.nb = 0;

	ALLOUER(tab_points_controle_gauche.table, tab_point_controle.nb);
	ALLOUER(tab_quad_calcul.table, tab_point_controle.nb);

	//On met les points de controle dans la table de calcul
	for(i = 0; i < tab_point_controle.nb; i++)
	{
		tab_quad_calcul.table[i] = tab_point_controle.table[i];
		tab_quad_calcul.table[i] = multiplicationParPoidsQuadQuad(tab_quad_calcul.table[i]);
	}

	//On récupère le premier point
	tab_points_controle_gauche.table[0] = tab_quad_calcul.table[0];
	tab_points_controle_gauche.nb++;

	//On récupère les autres points de controle en appliquand casteljau pour le u reparametré
	for(l = tab_point_controle.nb-1; l > 0; l--)
	{
		for(i = 0; i < l; i++)		
		{
			//Calcul
			tab_quad_calcul.table[i] = applicationFormuleCasteljau(tab_quad_calcul.table[i+1], tab_quad_calcul.table[i], u);
		}
		tab_points_controle_gauche.table[tab_point_controle.nb - l] = tab_quad_calcul.table[0];
		tab_points_controle_gauche.nb++;
	}

	//On récupère les vrais coordonnées
	for(i = 0; i < tab_points_controle_gauche.nb; i++)
	{
		tab_points_controle_gauche.table[i] = divisionParPoidsQuadQuad(tab_points_controle_gauche.table[i]);
	}

    free(tab_quad_calcul.table);
	
	return tab_points_controle_gauche;
}
