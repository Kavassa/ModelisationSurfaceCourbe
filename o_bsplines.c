
#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "bsplines.h"



#include "approximation.h"

struct bsplines
{
	int m_degre;
	Table_flottant m_vecteur_nodale;

	Table_quadruplet m_tab_points_controle;
	Table_quadruplet m_tab_points_courbe;

	int m_nb_points_a_afficher;

	Booleen m_affiche_points_controle;
} ; 


static void affiche_bsplines(struct bsplines* b)
{
	int j;


	//Affichage des points de controle
	if(b->m_affiche_points_controle)
	{
		glBegin(GL_LINE_STRIP) ;
		glColor3f(1.0, 1.0, 1.0);

		for(j = 0; j < b->m_tab_points_controle.nb; j++)
		{
			glVertex3f(	b->m_tab_points_controle.table[j].x, 
						b->m_tab_points_controle.table[j].y, 
						b->m_tab_points_controle.table[j].z);
		}
	
		glEnd();
	}


	glBegin(GL_POINTS);	
	glColor3f(1.0, 0.0, 0.0);

	//Affichage des points de la courbe
	for(j = 0; j < b->m_tab_points_courbe.nb; j++)
	{
		glVertex3f(	b->m_tab_points_courbe.table[j].x / b->m_tab_points_courbe.table[j].h, 
					b->m_tab_points_courbe.table[j].y / b->m_tab_points_courbe.table[j].h, 	
					b->m_tab_points_courbe.table[j].z / b->m_tab_points_courbe.table[j].h); 
	}

	glEnd();
}


static void changement(struct bsplines* b)
{
	if ( ! (UN_CHAMP_CHANGE(b)||CREATION(b)) )
		return ;  

	if ( CREATION(b) || CHAMP_CHANGE(b, m_tab_points_controle) || 
						CHAMP_CHANGE(b, m_nb_points_a_afficher) ||
						CHAMP_CHANGE(b, m_degre) || 
						CHAMP_CHANGE(b, m_vecteur_nodale))
	{
		if(b->m_degre >= b->m_tab_points_controle.nb)
			b->m_degre = b->m_tab_points_controle.nb - 1;
		else if(b->m_degre < 1)
			b->m_degre = 1;
		else
		{
			if(b->m_vecteur_nodale.nb == 0 || CHAMP_CHANGE(b, m_degre))
				b->m_vecteur_nodale = creationVecteurNodal( b->m_degre, b->m_tab_points_controle.nb);
			b->m_tab_points_courbe = calcul_bsplines(b->m_tab_points_controle, b->m_nb_points_a_afficher, b->m_vecteur_nodale, b->m_degre);
		}
	}
}


CLASSE(BSplines, struct bsplines,     
	CHAMP(m_tab_points_controle.nb, LABEL("Nombre de points de controle: ") L_entier Affiche) 
	CHAMP(m_nb_points_a_afficher, LABEL("Nombre de points à afficher: ") L_entier Affiche Edite Sauve DEFAUT("30"))  
	CHAMP(m_degre, LABEL("Degré: ") L_entier Affiche Edite Sauve DEFAUT("3"))  
	CHAMP(m_vecteur_nodale, LABEL("Vecteur nodale: ") L_table_flottant Affiche Edite Sauve)
	CHAMP(m_affiche_points_controle, LABEL("Affichage des points de contrôle") L_booleen Edite DEFAUT("0"))

	CHAMP(m_tab_points_controle, LABEL("Point") L_table_point P_table_quadruplet Extrait Obligatoire Edite Sauve)
    
	CHANGEMENT(changement)
	CHAMP_VIRTUEL(L_affiche_gl(affiche_bsplines))

	MENU("TP3/TP_BSplines")
	EVENEMENT("Ctrl+BS")

	)
