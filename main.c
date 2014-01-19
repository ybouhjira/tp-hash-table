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
  struct Entree **vals;
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
  if(!(table.vals = calloc(size ,sizeof(Entree))))
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

/* Create a key-value pair. */
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
void ht_set(TableHashage table, char *cle, int val) {
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

/* Retrieve a key-value pair from a hash table. */
int ht_get(TableHashage table, char *key) {
  int bin = 0;
  Entree *pair;

  bin = convertir_chaine(table, key);

  /* Step through the bin, looking for our value. */
  pair = table.vals[bin];
  while(pair && strcmp(key, pair->cle) > 0) pair = pair->suiv;

  /* Did we actually find anything? */
  if(!pair && strcmp(key, pair->cle) != 0)
    return 0;
  else
    return pair->val;
}


int main()
{
  TableHashage hashtable = tablehashage_creer(65536);

  ht_set(hashtable, "key1", 1);
  ht_set(hashtable, "key2", 2);
  ht_set(hashtable, "key3", 5);
  ht_set(hashtable, "key4", 4);

  printf("%d\n", ht_get(hashtable, "key1"));
  printf("%d\n", ht_get(hashtable, "key2"));
  printf("%d\n", ht_get(hashtable, "key3"));
  printf("%d\n", ht_get(hashtable, "key4"));

  return 0;
}
