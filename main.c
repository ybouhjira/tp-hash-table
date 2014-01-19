#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#define TAILLE_CLE 100
/**
  * Une entrée dans le table de hashage
  */
typedef struct Entree
{
  char cle[TAILLE_CLE];
  int val;
  struct Entree *suiv;
}Entree;

/**
  * La table de hashage
  */
typedef struct TableHashage
{
  int size;
  struct Entree* *vals;
}TableHashage;

/** Nom de la fonction : tablehashage_creer
  * Entrées :
  *   int size : La taille de de la table
  * Sorties :
  *   TableHashage *table : La table crée
  * Description :
  *   Crée une table de hashage qui a la taille size
  */
TableHashage tablehashage_creer(int size)
{
  assert(size >= 1);
  TableHashage table = {size, NULL};

  /* Allocation de la mémoire */
  table.vals = calloc(size, sizeof(Entree*));
  if(!table.vals)
    {
      printf("Erreur d'allocation de mémoire\n");
      exit(1);
    }

  return table;
}

/** Nom de la fonction : convertir_chaine
  * Entrées :
  *   TableHashage *table : La table de hashage
  * Sorties :
  *   int index : converti une cle
  * Description :
  *   convertit une cle en indice
  */
unsigned int convertir_chaine(TableHashage table, char *cle)
{
  unsigned int index;
  unsigned int i = 0;

  while(index < ULONG_MAX && i < strlen(cle))
    {
      index = index << 8;
      index += cle[i];
      i++;
    }

  return index % table.size;
}

/** Nom de la fonction : entree_creer
  * Entrées :
  *   char *cle : La clé
  *   int val : La valeur
  * Sorties :
  *   Entree nouv : L'entrée creée
  * Description :
  *   Crée une Entree en allouant de la mémoire avec malloc
  */
Entree *entree_creer(char *cle, int val) {
  Entree *nouv;

  if(!(nouv = calloc(1, sizeof(Entree))))
    {
      printf("Erreur d'allocation dynamique\n");
      exit(1);
    }
  strcpy(nouv->cle, cle);
  nouv->val = val;

  return nouv;
}

/** Nom de la fonction :  tablehashage_inserer
  * Entrées :
  *   TableHashage table : Table de hashage
  *   char *cle : La clé
  *   int val : La valeur
  * Description :
  *   insere une valeur dans la table de hashage
  */
void tablehashage_inserer(TableHashage table, char *cle, int val) {
  assert(cle);

  int indice = 0;
  Entree *nouv = NULL;
  Entree *suiv = NULL;
  Entree *prec = NULL;

  indice = convertir_chaine(table, cle);

  suiv = table.vals[indice];

  while(suiv && strcmp(cle, suiv->cle) > 0)
    {
      prec = suiv;
      suiv = suiv->suiv;
    }

  // La clé existe deja
  if(suiv && !strcmp(cle, suiv->cle))  suiv->val = val;
  else
    {
      nouv = entree_creer(cle, val);

      // tete de la liste chainée
      if(suiv == table.vals[indice])
        {
          nouv->suiv = suiv;
          table.vals[indice] = nouv;
        }
      // fin de la liste chainée
      else if (suiv == NULL) prec->suiv = nouv;
      // milieu de la liste
      else
        {
          nouv->suiv = suiv;
          prec->suiv = nouv;
        }
    }
}

/** Nom de la fonction : tablehashage_acceder
  * Entrées :
  *   TableHashage table : Une table de hashage
  *   char *cle : Une clé
  * Sorties :
  *   int pair : L'entrée contenant la valeur recherché
  * Description :
  *   Retourne l'entrée contenant la valeur qui la clé cle ou NULL
  * si elle n'existe pas.
  */
Entree* tablehashage_acceder(TableHashage table, char *cle)
{
  int indice = 0;
  Entree *pair;

  indice = convertir_chaine(table, cle);

  // Parcourir la liste chainée
  pair = table.vals[indice];
  while(pair && strcmp(cle, pair->cle) > 0) pair = pair->suiv;

  // Test si on a retrouvé la valeur
  if(!pair || strcmp(cle, pair->cle) != 0) return NULL;
  return pair;
}

/** Nom de la fonction : tablehashage_afficher
  * Entrées :
  *   TableHashage table : Une table de hashage.
  * Description :
  *   Affiche le contenu de ma table de hashage.
  */
void tablehashage_afficher(TableHashage table)
{
  int i;
  for(i = 0; i<table.size; ++i)
    {
      if(table.vals[i])
        {
          // Parcourir la liste chainée
          Entree *courant = table.vals[i];

          while(courant)
            {
              printf("\"%s\" : %d\n", table.vals[i]->cle, table.vals[i]->val);
              courant = courant->suiv;
            }
        }
    }
}

int main()
{

  int taille = 0;
  while(taille < 1)
    {
      printf("Taille de la table : ");
      scanf("%d", &taille);
    }

  TableHashage table = tablehashage_creer(taille);

  int choix = 1;

  while(1 <= choix && choix <= 4)
    {
      printf("\nMenu --------------------------\n"
             "    1 - Afficher la table\n"
             "    2 - Insérer une valeur\n"
             "    3 - Supprimer une valeur\n"
             "    4 - Accéder à une valeur\n"
             "Autre - Quitter\n"
             "Choix : ");
      scanf("%d", &choix);
      printf("-------------------------------\n\n");
      switch(choix)
        {
        case 1:
          tablehashage_afficher(table);
          break;
        case 2:
          {
            int valeur;
            char cle[TAILLE_CLE];
            printf("Clé : ");
            while(getchar() != '\n');
            scanf("%[^\n]s", cle);
            printf("Valeur : ");
            scanf("%d", &valeur);

            tablehashage_inserer(table, cle, valeur);
            break;
          }
        case 3:
          {
            // Pas de fonction de Suppression !!
            break;
          }
        case 4:
          {
            char cle[TAILLE_CLE];
            printf("Clé : ");
            while(getchar() != '\n');
            scanf("%[^\n]s", cle);

            Entree *entree = tablehashage_acceder(table, cle);
            if(entree) printf("Valeur : %d\n", entree->val);
            else printf("Aucune valeur ne correspond a cette clé\n");
            break;
          }
        }
    }
  return 0;
}
