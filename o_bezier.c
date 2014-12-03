
#include <GL/gl.h>
#include <math.h>
#include "o_objet.h"
#include "u_table.h"
#include "t_geometrie.h"
#include "bezier.h"

struct bezier
{
	int m_nb_points_a_afficher;
	double m_interval_min;
	double m_interval_max;

	Table_quadruplet m_tab_points_controle;
	Table_triplet m_tab_points_courbe;
	Table_quadruplet m_tab_points_controle_param;
	Table_triplet m_tab_points_courbe_param;

	Booleen m_affiche_points_controle;
	Booleen m_affiche_nouveaux_points_controle;
	Booleen m_affiche_nouvelle_courbe;
} ; 

static void affiche_bezier(struct bezier* b)
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
	if(b->m_affiche_nouvelle_courbe)
	{
		//Affichage des points de la courbe parametre
		for(j = 0; j < b->m_tab_points_courbe_param.nb; j++)
		{
			glVertex3f(	b->m_tab_points_courbe_param.table[j].x, 
						b->m_tab_points_courbe_param.table[j].y, 	
						b->m_tab_points_courbe_param.table[j].z); 
		}
	}
	else
	{
		//Affichage des points de la courbe
		for(j = 0; j < b->m_tab_points_courbe.nb; j++)
		{
			glVertex3f(	b->m_tab_points_courbe.table[j].x, 
						b->m_tab_points_courbe.table[j].y, 	
						b->m_tab_points_courbe.table[j].z); 
		}
	}
	glEnd();


	//Affichage des nouveaux points
	if(b->m_affiche_nouveaux_points_controle)
	{
		glBegin(GL_LINE_STRIP);

		glColor3f(0.0, 1.0, 0.0);
		if(b->m_tab_points_controle_param.nb != 0)	
		{
			for(j = 0; j < b->m_tab_points_controle_param.nb; j++)
			{
				glVertex3f(	b->m_tab_points_controle_param.table[j].x,
					 		b->m_tab_points_controle_param.table[j].y,
				 			b->m_tab_points_controle_param.table[j].z);
			}
		}
		glEnd();
	}
}





static void changement(struct bezier* b)
{
	if ( ! (UN_CHAMP_CHANGE(b)||CREATION(b)) )
		return ;  

	if ( CREATION(b) || CHAMP_CHANGE(b, m_tab_points_controle) || 
				CHAMP_CHANGE(b, m_nb_points_a_afficher) ||
				CHAMP_CHANGE(b, m_interval_min) ||
				CHAMP_CHANGE(b, m_interval_max))
	{
		if(b->m_tab_points_courbe.table != NULL)		
			free(b->m_tab_points_courbe.table);
		if(b->m_tab_points_controle_param.table != NULL)
			free(b->m_tab_points_controle_param.table);

		/*b->m_tab_points_courbe = calcul_bezier(b->m_tab_points_controle, b->m_nb_points_a_afficher);
		b->m_tab_points_controle_param = calculPointsCourbeInterval(b->m_tab_points_controle, b->m_interval_min, b->m_interval_max);
		b->m_tab_points_courbe_param = calcul_bezier(b->m_tab_points_controle_param, b->m_nb_points_a_afficher);*/

		Table_quadruplet m_tab_points_courbe_temp;
		Table_quadruplet m_tab_points_courbe_param_temp;

		m_tab_points_courbe_temp = calcul_bezier(b->m_tab_points_controle, b->m_nb_points_a_afficher);
		int i;
		ALLOUER(b->m_tab_points_courbe.table, m_tab_points_courbe_temp.nb);
		for(i = 0; i < m_tab_points_courbe_temp.nb; i++)
		{
			b->m_tab_points_courbe.table[i] = divisionParPoidsQuadTripl(m_tab_points_courbe_temp.table[i]);
		}
		b->m_tab_points_courbe.nb = m_tab_points_courbe_temp.nb;

		b->m_tab_points_controle_param = calculPointsCourbeInterval(b->m_tab_points_controle, b->m_interval_min, b->m_interval_max);
		
		m_tab_points_courbe_param_temp = calcul_bezier(b->m_tab_points_controle_param, b->m_nb_points_a_afficher);
		ALLOUER(b->m_tab_points_courbe_param.table, m_tab_points_courbe_param_temp.nb);
		for(i = 0; i < m_tab_points_courbe_param_temp.nb; i++)
		{
			b->m_tab_points_courbe_param.table[i] = divisionParPoidsQuadTripl(m_tab_points_courbe_param_temp.table[i]);
		}
		b->m_tab_points_courbe_param.nb = m_tab_points_courbe_param_temp.nb;
	}
}






CLASSE(Bezier, struct bezier,      
	CHAMP(m_tab_points_controle.nb, LABEL("Nombre de points de controle: ") L_entier Affiche)
	CHAMP(m_nb_points_a_afficher, LABEL("Nombre de points à afficher: ") L_entier /*Extrait*/ Affiche Edite Sauve DEFAUT("50"))  
	CHAMP(m_affiche_points_controle, LABEL("Affichage des points de contrôle") L_booleen Edite DEFAUT("0"))
	CHAMP(m_affiche_nouveaux_points_controle, LABEL("Affichage des nouveaux points de contrôle") L_booleen Edite DEFAUT("0"))
	CHAMP(m_affiche_nouvelle_courbe, LABEL("Affichage de la nouvelle courbe") L_booleen Edite DEFAUT("0"))

	CHAMP(m_tab_points_controle, LABEL("Point") L_table_point P_table_quadruplet Extrait Obligatoire Edite Sauve)

	CHAMP(m_interval_min, LABEL("Interval min: ") L_flottant Edite DEFAUT("0.3"))
	CHAMP(m_interval_max, LABEL("Interval max: ") L_flottant Edite DEFAUT("0.7"))    
    
	CHANGEMENT(changement)
	CHAMP_VIRTUEL(L_affiche_gl(affiche_bezier))

	MENU("TP1/TP_Bezier")
	EVENEMENT("Ctrl+BZ")
	)
