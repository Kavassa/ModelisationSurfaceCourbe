#include "bsplines.h"



Table_flottant creationVecteurNodal( int degre, int nb_point_controle)
{
	int dernier_ind_PC = nb_point_controle - 1;
	int ordre = degre + 1;
	
	Table_flottant tab;
	ALLOUER(tab.table, dernier_ind_PC + ordre + 1)

	int i;
	for(i = 0; i <= ordre-1; i++)
	{
		tab.table[i] = 0.0;
	}
	for(i = ordre; i <= dernier_ind_PC; i++)
	{
		tab.table[i] = (double)(i - ordre + 1) / (dernier_ind_PC - ordre + 2); 
	}
	for(i = dernier_ind_PC + 1; i <= dernier_ind_PC + ordre; i++)
	{
		tab.table[i] = 1.0;
	}
	tab.nb = dernier_ind_PC+ordre+1;
	return tab;	
}


int retourneIndiceVectreurNodale(Table_flottant tab, int degre, int nb_point_controle, double u)
{
	int i;
	int dernier_ind_PC = nb_point_controle - 1;
	int ordre = degre + 1;

	for(i = ordre - 1; i <= dernier_ind_PC; i++ )
	{
		if((int) u == 1) 
		{
			return dernier_ind_PC;
		}
		if( tab.table[i] <= u && u <= tab.table[i+1])
		{
			return i;
		}
	}
	return -1;
}





Table_quadruplet calcul_bsplines(Table_quadruplet tab_point_controle, int nb_points_a_afficher, Table_flottant vector_nodal, int degre)
{
	int i;
	int j;
	int h;
	int r;
	int ordre = degre + 1;

	double u = 0;

	//Tableau des points resultats représentant la courbe
	Table_quadruplet tab_quadruplet_courbe;
	tab_quadruplet_courbe.nb = 0;
	//Tableau que l'on va utiliser pour stocker les calculs
	Table_quadruplet tab_quad_calcul;
	//Allocation des deux tableaux
	ALLOUER(tab_quadruplet_courbe.table, nb_points_a_afficher);
	ALLOUER(tab_quad_calcul.table, tab_point_controle.nb);

	for(h = 0; h < nb_points_a_afficher; h++)
	{	
		//On stocke les points de controle dans le tableau de calcul
		for(j = 0; j < tab_point_controle.nb; j++)
		{
			tab_quad_calcul.table[j] = multiplicationParPoidsQuadQuad(tab_point_controle.table[j]);
		}
		//On met a jour le nombre d'éléments dans le tableau
		tab_quad_calcul.nb = tab_point_controle.nb;

		//######################################
		//On applique les formules du cour
		//######################################
		r = retourneIndiceVectreurNodale(vector_nodal, degre, tab_point_controle.nb, u);

		for(j = 1; j <= ordre - 1; j++)
		{
			for(i = r; i >= r - ordre + 1 + j; i--)
			{
				tab_quad_calcul.table[i].x = tab_quad_calcul.table[i].x * ( (u - vector_nodal.table[i]) / (vector_nodal.table[i + ordre - j] - vector_nodal.table[i]) ) 
						 				   + tab_quad_calcul.table[i-1].x * ( (vector_nodal.table[i + ordre - j] - u) / (vector_nodal.table[i + ordre -j] - vector_nodal.table[i]) ) ;
				tab_quad_calcul.table[i].y = tab_quad_calcul.table[i].y * ( (u - vector_nodal.table[i]) / (vector_nodal.table[i + ordre - j] - vector_nodal.table[i]) ) 
						 				   + tab_quad_calcul.table[i-1].y * ( (vector_nodal.table[i + ordre - j] - u) / (vector_nodal.table[i + ordre -j] - vector_nodal.table[i]) ) ;
				tab_quad_calcul.table[i].z = tab_quad_calcul.table[i].z * ( (u - vector_nodal.table[i]) / (vector_nodal.table[i + ordre - j] - vector_nodal.table[i]) ) 
						 				   + tab_quad_calcul.table[i-1].z * ( (vector_nodal.table[i + ordre - j] - u) / (vector_nodal.table[i + ordre -j] - vector_nodal.table[i]) ) ;
				tab_quad_calcul.table[i].h = tab_quad_calcul.table[i].h * ( (u - vector_nodal.table[i]) / (vector_nodal.table[i + ordre - j] - vector_nodal.table[i]) ) 
						 				   + tab_quad_calcul.table[i-1].h * ( (vector_nodal.table[i + ordre - j] - u) / (vector_nodal.table[i + ordre -j] - vector_nodal.table[i]) ) ;
			}
		}

		//On stocke le nouveau point créé
		tab_quadruplet_courbe.table[h] = tab_quad_calcul.table[r];
		tab_quadruplet_courbe.nb++;

		u += (double)1/(nb_points_a_afficher-1);
	}
	
	return tab_quadruplet_courbe;
}
























