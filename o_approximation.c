
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
	Booleen m_uniforme;
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


	glBegin(GL_LINE_STRIP);	
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
						CHAMP_CHANGE(a, m_tab_points_approx) ||
						CHAMP_CHANGE(a, m_uniforme))
	{
		if(a->m_degre < 0)
			a->m_degre = 0;
		else if (a->m_degre >= a->m_tab_points_approx.nb)
			a->m_degre = a->m_tab_points_approx.nb -1;

		a->m_tab_points_point_controle = calcul(a->m_degre, a->m_tab_points_approx, a->m_uniforme);
		a->m_tab_courbe_Approx = calcul_bezier(a->m_tab_points_point_controle, a->m_nb_points_a_afficher);
	}
}


CLASSE(Approximation, struct approximation,     
	CHAMP(m_degre, LABEL("Degré: ") L_entier Affiche Edite Sauve DEFAUT("2"))  
	CHAMP(m_nb_points_a_afficher, LABEL("Nombre de points à afficher: ") L_entier Affiche Edite Sauve DEFAUT("30"))  
	CHAMP(m_affiche_points_controle, LABEL("Affichage des points de contrôle") L_booleen Edite DEFAUT("0"))
	CHAMP(m_tab_points_approx, LABEL("Point") L_table_point P_table_quadruplet Extrait Obligatoire Edite Sauve)
	CHAMP(m_uniforme, LABEL("Uniforme") L_booleen Edite DEFAUT("1"))

	CHANGEMENT(changement)
	CHAMP_VIRTUEL(L_affiche_gl(affiche_approximation))

	MENU("TP4/TP_Approximation")
	EVENEMENT("Ctrl+APXT")

	)
