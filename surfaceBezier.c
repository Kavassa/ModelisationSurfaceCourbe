#include "surfaceBezier.h"



Grille_quadruplet calcul_surface_bezier(Grille_quadruplet grille_points_controle, int nb_points_a_afficher)
{
	int i;
	int j;
	int k;

	Grille_quadruplet grille_nouveau_points_controle; //Grille de tous les nouveau points de controle a partir des points de courbe
	Grille_quadruplet grille_finale;
	Table_quadruplet tab_points_controle; //Stocke tab de point de controle pour le casteljau courant
	Table_quadruplet tab_points_courbe; //Stocke les points de la courbe en sortie de casteljau
	ALLOUER(tab_points_controle.table, grille_points_controle.nb_colonnes);

	ALLOUER(grille_nouveau_points_controle.grille, grille_points_controle.nb_lignes);
	for (i=0 ; i<grille_points_controle.nb_lignes ; i++)
		ALLOUER(grille_nouveau_points_controle.grille[i],nb_points_a_afficher);

	grille_finale.nb_lignes = nb_points_a_afficher;
	grille_finale.nb_colonnes = nb_points_a_afficher;

	ALLOUER(grille_finale.grille, nb_points_a_afficher);
	for (i=0 ; i<nb_points_a_afficher ; i++)
		ALLOUER(grille_finale.grille[i],nb_points_a_afficher);

	for(j = 0; j < grille_points_controle.nb_lignes; j++)
	{
		for(k = 0; k < grille_points_controle.nb_colonnes; k++)
		{
			tab_points_controle.table[k] = grille_points_controle.grille[j][k]; //On copie les points de controle pour casteljau
		}
		tab_points_controle.nb = grille_points_controle.nb_colonnes;

		tab_points_courbe = calcul_bezier(tab_points_controle, nb_points_a_afficher);
		
		for(k = 0; k < tab_points_courbe.nb; k++)
		{
			grille_nouveau_points_controle.grille[j][k] =  tab_points_courbe.table[k];
		}
		grille_nouveau_points_controle.nb_colonnes = tab_points_courbe.nb;
		grille_nouveau_points_controle.nb_lignes = grille_points_controle.nb_lignes;
	}

	for(i = 0; i < grille_nouveau_points_controle.nb_lignes; i++)
	{
		for(j = 0; j < grille_nouveau_points_controle.nb_colonnes; j++)
		{
			grille_nouveau_points_controle.grille[i][j] = divisionParPoidsQuadQuad(grille_nouveau_points_controle.grille[i][j]);
		}
	}

	free(tab_points_controle.table);
	ALLOUER(tab_points_controle.table, grille_nouveau_points_controle.nb_colonnes);

	for(i = 0; i < grille_nouveau_points_controle.nb_colonnes; i++)
	{
		for(k = 0; k < grille_nouveau_points_controle.nb_lignes; k++)
		{
			tab_points_controle.table[k] = grille_nouveau_points_controle.grille[k][i]; //On copie les points de controle pour casteljau
		}
		tab_points_controle.nb = grille_nouveau_points_controle.nb_lignes;

		tab_points_courbe = calcul_bezier(tab_points_controle, nb_points_a_afficher);

		for(k = 0; k < tab_points_courbe.nb; k++)
		{
			grille_finale.grille[k][i] =  tab_points_courbe.table[k];
		}
	}
	grille_finale.nb_lignes = nb_points_a_afficher;
	grille_finale.nb_colonnes = nb_points_a_afficher;

	return grille_finale;

	//declaration de grille de point de controle secondaire
	//Pour chaque ligne
		//on recupere le tableau de point de controle
		//Je calcul bezier pour le tabl
		//je stocke le resultat dans la grille secondaire
	//Pour chaque colonne du secondaire
		//Je recupere un tableau
		//Je bezier dessus
		//je stocke resultat dans grille final
	//je return la grille
}