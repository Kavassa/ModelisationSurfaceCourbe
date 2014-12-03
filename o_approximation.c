
#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "approximation.h"
#include "bezier.h"

struct approximation
{
	Table_quadruplet m_tab_points_approx;
	Table_quadruplet m_tab_points_point_controle;
	Table_quadruplet m_tab_courbe_Approx;
	int m_nb_points_a_afficher;
	int m_degre;
	Booleen m_affiche_points_controle;
}; 


static void affiche_approximation(struct approximation* a)
{
	int j;

	glBegin(GL_LINE_STRIP) ;
	glColor3f(1.0, 1.0, 1.0);

	//Affichage des points de controle
	if(a->m_affiche_points_controle)
	{
		for(j = 0; j < a->m_tab_points_point_controle.nb; j++)
		{
			glVertex3f(	a->m_tab_points_point_controle.table[j].x, 
						a->m_tab_points_point_controle.table[j].y, 
						a->m_tab_points_point_controle.table[j].z);
		}
	}

	glEnd();


	glBegin(GL_POINTS);	
	glColor3f(1.0, 0.0, 0.0);

	//Affichage des points de la courbe
	for(j = 0; j < a->m_tab_courbe_Approx.nb; j++)
	{
		glVertex3f(	a->m_tab_courbe_Approx.table[j].x / a->m_tab_courbe_Approx.table[j].h, 
					a->m_tab_courbe_Approx.table[j].y / a->m_tab_courbe_Approx.table[j].h, 	
					a->m_tab_courbe_Approx.table[j].z / a->m_tab_courbe_Approx.table[j].h); 
	}

	glEnd();
}


static void changement(struct approximation* a)
{
	if ( ! (UN_CHAMP_CHANGE(a)||CREATION(a)) )
		return ;  

	if ( CREATION(a) || CHAMP_CHANGE(a, m_degre) || 
						CHAMP_CHANGE(a, m_nb_points_a_afficher) ||
						CHAMP_CHANGE(a, m_tab_points_approx))
	{
		// fprintf(stderr, "factoriel: 5! = %d\n", factorielle(5)); /*OK*/
		// fprintf(stderr, "bernstein: %f\n", bernstein(2, 2, 0.5)); /*OK*/
		// Grille_flottant res = creerMatriceCoeffBernstein(2,4);
		// Grille_flottant res_trans = creerMatriceCoeffBernstein(2,4);
		
		// Table_quadruplet tab;
		// ALLOUER(tab.table,3);
		// tab.nb = 3;
		// tab.table[0].x = 1;
		// tab.table[1].x = 2;
		// tab.table[2].x = 3;

		// tab.table[0].y = 4;
		// tab.table[1].y = 5,
		// tab.table[2].y = 6;
		
		// tab.table[0].z = 7;
		// tab.table[1].z = 8;
		// tab.table[2].z = 9;
		
		// tab.table[0].h = 10;
		// tab.table[1].h = 11;
		// tab.table[2].h = 12;
		// Table_flottant x = getCoordX(tab);
		// Table_flottant y = getCoordY(tab);
		// Table_flottant z = getCoordZ(tab);
		// affichageTableFlottant(x);
		// affichageTableFlottant(y);
		// affichageTableFlottant(z);

		// fprintf(stderr, "matrice:\n");
		// affichageGrilleFlottant(res);
		
		// res_trans = transposition_matrice(res);
		// fprintf(stderr, "transposé:\n");
		// affichageGrilleFlottant(res_trans);
		
		// res = multiplication_matrice_matrice(res, res_trans);
		// fprintf(stderr, "multiplication:\n");
		// affichageGrilleFlottant(res);

		// Table_flottant x = getCoordX(a->m_tab_points_approx);
		// Table_flottant result = multiplication_matrice_vecteur(res, x);
		// affichageTableFlottant(result);

		a->m_tab_points_point_controle = calcul(a->m_degre, a->m_tab_points_approx);
		a->m_tab_courbe_Approx = calcul_bezier(a->m_tab_points_approx, a->m_nb_points_a_afficher);
		
		//affichageTableQuadruplet(&point_controle);


	}
}


CLASSE(Approximation, struct approximation,     
	CHAMP(m_degre, LABEL("Degré: ") L_entier Affiche Edite Sauve DEFAUT("2"))  
	CHAMP(m_nb_points_a_afficher, LABEL("Nombre de points à afficher: ") L_entier Affiche Edite Sauve DEFAUT("30"))  
	CHAMP(m_affiche_points_controle, LABEL("Affichage des points de contrôle") L_booleen Edite DEFAUT("0"))
	CHAMP(m_tab_points_approx, LABEL("Point") L_table_point P_table_quadruplet Extrait Obligatoire Edite Sauve)

	CHANGEMENT(changement)
	CHAMP_VIRTUEL(L_affiche_gl(affiche_approximation))

	MENU("TP4/TP_Approximation")
	EVENEMENT("Ctrl+APXT")

	)
