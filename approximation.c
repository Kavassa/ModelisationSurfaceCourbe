#include "o_objet.h"
#include "t_geometrie.h"
#include <math.h>
#include "approximation.h"

#define ZERO 1E-08

//################################# DEBUT ##################################

Grille_flottant duplique_Grille_flottant(Grille_flottant *original)
{
   int i,j;  
   Grille_flottant copie;

   ALLOUER(copie.grille,original->nb_lignes);
   copie.nb_lignes = original->nb_lignes;
   copie.nb_colonnes = original->nb_colonnes;
   
   for (i=0 ; i<original->nb_lignes ; i++)
     {
      ALLOUER(copie.grille[i],original->nb_colonnes);
      for (j=0 ; j<original->nb_colonnes ; j++)
            copie.grille[i][j] = original->grille[i][j];
     }

   return copie;
}

/*-------------------------------------------------------------------------*/

Table_flottant duplique_Table_flottant(Table_flottant *original)
{
   int i;  
   Table_flottant copie;

   ALLOUER(copie.table,original->nb);
   copie.nb = original->nb ;
   
   for (i=0 ; i<original->nb ; i++)     
            copie.table[i] = original->table[i];

   return copie;
}

/*-------------------------------------------------------------------------*/
void traite_pivot_max(Grille_flottant  *A,Table_flottant  *f, int colonne)
{
  int i,indice;
  double pivot,tps;
  Flottant *tmp;

  indice = colonne;
  pivot = fabs(A->grille[colonne][colonne]);

  //recherche pivot max

  for (i=colonne+1 ; i<A->nb_lignes ; i++)
    if (fabs(A->grille[i][colonne]) > pivot)
      {
	pivot = fabs(A->grille[i][colonne]);
	indice = i;
      }

  // traite pivot max

  if (indice != colonne)
    { 
      tmp = A->grille[colonne];
      A->grille[colonne] = A->grille[indice];
      A->grille[indice] = tmp;

      tps = f->table[colonne];
      f->table[colonne] = f->table[indice];
      f->table[indice] = tps;
    }
  
}

/*-------------------------------------------------------------------------*/
int resolution_systeme_lineaire(Grille_flottant  *A, 
                                Table_flottant  *f, 
                                Table_flottant  *x)
/*-------------------------------------------------------------------------*/
/* 
   Resolution du systeme lineaire   Ax=f par  methode de gauss
*/
/*-------------------------------------------------------------------------*/
{
 double  s,r;
 int i,j,k,n;
 Grille_flottant T;
 Table_flottant  b;
 

   n = A->nb_lignes; 
   if ( (A->nb_lignes != A->nb_colonnes) || (f->nb != A->nb_lignes) ) 
     return 1;

/* copie de la matrice et du second membre pour triangulation  */
   T = duplique_Grille_flottant(A);
   b = duplique_Table_flottant(f);

   /* allocation  solution */ 
   ALLOUER(x->table,n);
   x->nb = n; 

   for (k=0 ; k<n ; k++)
     {
       traite_pivot_max(&T,&b,k);
       if ( fabs(T.grille[k][k]) < ZERO)
         {
          free(x->table);
	  x->nb = 0;
          return(1);
	 }
       for (i=k+1 ; i<n ; i++)
       {   
          
         s = T.grille[i][k]/T.grille[k][k];	
         for (j=k+1 ; j<n ; j++)
             T.grille[i][j]=T.grille[i][j] - T.grille[k][j]*s;	
         b.table[i] -= s*b.table[k]; 
       }
     }

   if (fabs(T.grille[n-1][n -1]) < ZERO)  
   {
       free(x->table);
       x->nb = 0;
       return(1);
   }  

   x->table[n-1] = b.table[n-1]/T.grille[n-1][n-1];
   for (i=n-2 ; i>=0 ; i--)
   {
      r = 0.0;
      for (j=i+1 ; j<n ; j++)
         r += (T.grille[i][j])*(x->table[j]);
      if (fabs(T.grille[i][i]) < ZERO)   return(1);  
      x->table[i] =  (b.table[i] - r)/T.grille[i][i]; 
   }



   free(b.table);
   for (i=0 ; i<T.nb_lignes ; i++)
     free(T.grille[i]);
   free(T.grille);
   
   return(0); 
} 
/*-------------------------------------------------------------------------*/



//################################# FIN ####################################






Table_flottant getCoordX(Table_quadruplet* tab)
{
	int i;
	Table_flottant res;
	ALLOUER(res.table, tab->nb);
	res.nb = tab->nb;
	for(i = 0; i < tab->nb; i++)
	{
		res.table[i] = tab->table[i].x;
	}
	return res;
}

Table_flottant getCoordY(Table_quadruplet* tab)
{
	int i;
	Table_flottant res;
	ALLOUER(res.table, tab->nb);
	res.nb = tab->nb;
	for(i = 0; i < tab->nb; i++)
	{
		res.table[i] = tab->table[i].y;
	}
	return res;
}

Table_flottant getCoordZ(Table_quadruplet* tab)
{
	int i;
	Table_flottant res;
	ALLOUER(res.table, tab->nb);
	res.nb = tab->nb;
	for(i = 0; i < tab->nb; i++)
	{
		res.table[i] = tab->table[i].z;
	}
	return res;
}

void setCoordX(Table_quadruplet* tab_quad, Table_flottant* tab_float)
{
	int i;
	for(i = 0; i < tab_float->nb; i++)
	{
		tab_quad->table[i].x = tab_float->table[i];
	}
}

void setCoordY(Table_quadruplet* tab_quad, Table_flottant* tab_float)
{
	int i;
	for(i = 0; i < tab_float->nb; i++)
	{
		tab_quad->table[i].y = tab_float->table[i];
	}
}

void setCoordZ(Table_quadruplet* tab_quad, Table_flottant* tab_float)
{
	int i;
	for(i = 0; i < tab_float->nb; i++)
	{
		tab_quad->table[i].z = tab_float->table[i];
	}
}

void setCoordHa1(Table_quadruplet* tab_quad)
{
	int i;
	for(i = 0; i < tab_quad->nb; i++)
	{
		tab_quad->table[i].h = 1;
	}
}


Grille_flottant allocationMatrice( int ligne, int colonne)
{
	int i;
	Grille_flottant res;
	ALLOUER(res.grille, ligne);
	res.nb_lignes = ligne;

	for(i = 0; i < res.nb_lignes; i++)
	{
		ALLOUER(res.grille[i], colonne);
	}
	res.nb_colonnes = colonne;
	return res;
}


Grille_flottant multiplication_matrice_matrice(Grille_flottant* matrice_1, Grille_flottant* matrice_2)
{
	int j;
	int i;
	int k;
	double total;
	Grille_flottant res;

	res = allocationMatrice(matrice_1->nb_lignes, matrice_2->nb_colonnes);

	for(j = 0; j < matrice_1->nb_lignes; j++)
	{
		for(i = 0; i < matrice_2->nb_colonnes; i++)
		{
			total = 0.0;
			for(k = 0; k < matrice_2->nb_lignes; k++)
			{
				total += matrice_1->grille[j][k] * matrice_2->grille[k][i];
			}
			res.grille[j][i] = total;
		}
	}
	return res;
}

Table_flottant multiplication_matrice_vecteur(Grille_flottant* g, Table_flottant* f)
{
	int i;
	int j;
	double total;
	Table_flottant res;
	ALLOUER(res.table, g->nb_lignes);
	res.nb = g->nb_lignes;
	for(i = 0; i < g->nb_lignes; i++)
	{
		total = 0.0;
		for(j = 0; j < f->nb; j++)
		{
			total += g->grille[i][j] * f->table[j];
		}
		res.table[i] = total;
	}
	return res;
}


Grille_flottant transposition_matrice(Grille_flottant* matrice)
{
	int j;
	int i;
	Grille_flottant res;

	res = allocationMatrice(matrice->nb_colonnes, matrice->nb_lignes);

	for(j = 0; j < res.nb_lignes; j++)
		for(i = 0; i < res.nb_colonnes; i++)
			res.grille[j][i] = matrice->grille[i][j];
	return res;
}



int factorielle( int n) 
{
	int i;
	int retour = n;
	if(n == 0)
		return 1;
	for(i = n-1; i > 0; i--)
		retour *= i;
	return retour;
}

double bernstein( int i,  int degre, double u)
{
	double pascal = (double)factorielle(degre)/( (double)factorielle(i)*(double)factorielle(degre-i) );
	double u1 = 1 - u;
	u1 = pow(u1, degre - i);
	u = pow(u, i);
	double res = pascal * u1 * u;
	return res;
}

double calculDistance(Quadruplet* q1, Quadruplet* q2)
{
	return sqrt(pow(q2->x - q1->x,2) + pow(q2->y - q1->y,2) + pow(q2->z - q1->z,2));
}

Grille_flottant creerMatriceCoeffBernstein( int degre,  Booleen uniforme, Table_quadruplet tab)
{
	int i;
	int j;
	double u;
	Grille_flottant mat;
	double somme_distance = 0.0;
	double somme_temp = 0.0;
	for(i = 0; i < tab.nb-1; i++)
	{
		somme_distance += calculDistance(&(tab.table[i]), &(tab.table[i+1]));
	}

	//Allocation de la matrice que l'on va remplir
	mat = allocationMatrice(degre+1, tab.nb); // enlever -1

	//On rempli la grille
	for( i = 0; i < mat.nb_lignes; i++)
	{
		somme_temp = 0.0;
		for(j = 0; j < mat.nb_colonnes; j++)
		{
			if(uniforme)
				u = (double)j/(tab.nb-1);
			else
			{
				if(j == 0)
					u = 0.0;
				else
				{
					somme_temp += calculDistance(&(tab.table[j-1]), &(tab.table[j])); 
					u = somme_temp / somme_distance;
				}
			}
			mat.grille[i][j] = bernstein(i, degre, u); // enlever -1 a nombrepoint
		}
	}
	return mat;
}

void affichageGrilleFlottant(Grille_flottant* g)
{
	int i;
	int j;
	for(i = 0; i < g->nb_lignes; i++)
	{
		for(j = 0; j < g->nb_colonnes; j++)
		{
			fprintf(stderr, "%f ", g->grille[i][j]);
		}
		fprintf(stderr, "\n");
	}
}

void affichageTableFlottant(Table_flottant* tab)
{
	int i;
	for(i = 0; i < tab->nb; i++)
	{
		fprintf(stderr, "%f ", tab->table[i]);
	}
	fprintf(stderr, "\n");
}

void affichageTableQuadruplet(Table_quadruplet* tab)
{
	int i;
	for(i = 0; i < tab->nb; i++)
	{
		fprintf(stderr, "%f %f %f %f \n", tab->table[i].x, tab->table[i].y, tab->table[i].z, tab->table[i].h);
	}
	fprintf(stderr, "\n");
}


Table_quadruplet calcul(int degre, Table_quadruplet tab, Booleen uniforme)
{
	// B
	Grille_flottant mat_bernstein = creerMatriceCoeffBernstein(degre, uniforme, tab);
	// Bt
	Grille_flottant mat_bernstein_trans = creerMatriceCoeffBernstein(degre, uniforme, tab);
	mat_bernstein_trans = transposition_matrice(&mat_bernstein);
	//Px Py Pz
	Table_flottant x = getCoordX(&tab);
	Table_flottant y = getCoordY(&tab);
	Table_flottant z = getCoordZ(&tab);
	
	Table_flottant resX;
	Table_flottant resY;
	Table_flottant resZ;
	
	//B * Px
	Table_flottant B_fois_Px = multiplication_matrice_vecteur(&mat_bernstein, &x);
	Table_flottant B_fois_Py = multiplication_matrice_vecteur(&mat_bernstein, &y);
	Table_flottant B_fois_Pz = multiplication_matrice_vecteur(&mat_bernstein, &z);
	//B * Btrans
	Grille_flottant B_fois_Btrans = multiplication_matrice_matrice(&mat_bernstein, &mat_bernstein_trans);
	
	int test;  
	test = resolution_systeme_lineaire(&B_fois_Btrans, &B_fois_Px, &resX);
	test = resolution_systeme_lineaire(&B_fois_Btrans, &B_fois_Py, &resY);
	test = resolution_systeme_lineaire(&B_fois_Btrans, &B_fois_Pz, &resZ);

	Table_quadruplet PC;
	ALLOUER(PC.table, resX.nb);
	PC.nb = resX.nb;
	setCoordX(&PC, &resX);
	setCoordY(&PC, &resY);
	setCoordZ(&PC, &resZ);
	setCoordHa1(&PC);

	return PC;

}











