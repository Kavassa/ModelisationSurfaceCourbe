
#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "surfaceBezier.h"

struct surfaceBezier
{
	int m_nb_points_a_afficher;

	Grille_quadruplet m_grille_points_controle;
	Grille_triplet m_grille_points_courbe;

	Booleen m_affiche_points_controle;
} ; 

static void affiche_surface_bezier(struct surfaceBezier* sb)
{

	int i;
	int j;
	
	//Affichage des points de controle
	if(sb->m_affiche_points_controle)
	{
		for(i = 0; i < sb->m_grille_points_controle.nb_lignes; i++)
		{
			glBegin(GL_LINE_STRIP);	
			glColor3f(1.0, 0.0, 0.0);
			for(j = 0; j < sb->m_grille_points_controle.nb_colonnes; j++)
			{
				glVertex3f(	sb->m_grille_points_controle.grille[i][j].x,
					 		sb->m_grille_points_controle.grille[i][j].y,
				 			sb->m_grille_points_controle.grille[i][j].z);

			}
			glEnd();
		}

		for(i = 0; i < sb->m_grille_points_controle.nb_colonnes; i++)
		{
			glBegin(GL_LINE_STRIP);	
			glColor3f(1.0, 0.0, 0.0);
			for(j = 0; j < sb->m_grille_points_controle.nb_lignes; j++)
			{
				glVertex3f(	sb->m_grille_points_controle.grille[j][i].x,
					 		sb->m_grille_points_controle.grille[j][i].y,
				 			sb->m_grille_points_controle.grille[j][i].z);

			}
			glEnd();
		}
	}


	
	
	for(i = 0; i < sb->m_grille_points_courbe.nb_lignes; i++)
	{
		glBegin(GL_LINE_STRIP);	
		glColor3f(1.0, 1.0, 1.0);
		
		for(j = 0; j < sb->m_grille_points_courbe.nb_colonnes; j++)
		{
			glVertex3f(	sb->m_grille_points_courbe.grille[i][j].x,
				 		sb->m_grille_points_courbe.grille[i][j].y,
			 			sb->m_grille_points_courbe.grille[i][j].z);
		}

		glEnd();
	}

	
	for(i = 0; i < sb->m_grille_points_courbe.nb_colonnes; i++)
	{
		glBegin(GL_LINE_STRIP);	
		glColor3f(1.0, 1.0, 1.0);
		for(j = 0; j < sb->m_grille_points_courbe.nb_lignes; j++)
		{
			glVertex3f(	sb->m_grille_points_courbe.grille[j][i].x,
				 		sb->m_grille_points_courbe.grille[j][i].y,
			 			sb->m_grille_points_courbe.grille[j][i].z);
		}
		glEnd();
	}

}





static void changement(struct surfaceBezier* sb)
{
	if ( ! (UN_CHAMP_CHANGE(sb)||CREATION(sb)) )
		return ;  

	if ( CREATION(sb) || CHAMP_CHANGE(sb, m_grille_points_controle) || 
				CHAMP_CHANGE(sb, m_nb_points_a_afficher) )
	{
		int i;
		int j;

		if(sb->m_grille_points_courbe.grille != NULL)
		{	
			for (i=0 ; i < sb->m_grille_points_courbe.nb_lignes ; i++)	
				free(sb->m_grille_points_courbe.grille[i]);
			free(sb->m_grille_points_courbe.grille);
		}

		Grille_quadruplet m_grille_points_courbe_temp;

		m_grille_points_courbe_temp = calcul_surface_bezier(sb->m_grille_points_controle, sb->m_nb_points_a_afficher);
	
		ALLOUER(sb->m_grille_points_courbe.grille, m_grille_points_courbe_temp.nb_lignes);
		for (i=0 ; i<m_grille_points_courbe_temp.nb_lignes ; i++)
			ALLOUER(sb->m_grille_points_courbe.grille[i],m_grille_points_courbe_temp.nb_colonnes);

		for(i = 0; i < m_grille_points_courbe_temp.nb_lignes; i++)
		{
			for(j = 0; j < m_grille_points_courbe_temp.nb_colonnes; j++)
			{
				sb->m_grille_points_courbe.grille[i][j] = divisionParPoidsQuadTripl(m_grille_points_courbe_temp.grille[i][j]);
			}
		}
		sb->m_grille_points_courbe.nb_lignes = m_grille_points_courbe_temp.nb_lignes;
		sb->m_grille_points_courbe.nb_colonnes = m_grille_points_courbe_temp.nb_colonnes; 
	}
}






CLASSE(SurfaceBezier, struct surfaceBezier,      
	//CHAMP(m_grille_points_controle.nb, LABEL("Nombre de points de controle: ") L_entier Affiche)
	CHAMP(m_nb_points_a_afficher, LABEL("Nombre de points à afficher: ") L_entier /*Extrait*/ Affiche Edite Sauve DEFAUT("30"))  
	CHAMP(m_affiche_points_controle, LABEL("Affichage des points de contrôle") L_booleen Edite DEFAUT("0"))

	CHAMP(m_grille_points_controle, LABEL("Point") L_grille_point P_grille_quadruplet Extrait Obligatoire Edite Sauve)  
    
	CHANGEMENT(changement)
	CHAMP_VIRTUEL(L_affiche_gl(affiche_surface_bezier))

	MENU("TP2/TP_Surface_Bezier")
	EVENEMENT("Ctrl+SBZ")

	)
