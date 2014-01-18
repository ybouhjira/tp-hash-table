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
  struct Entree *vals;
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
unsigned int convertir_chaine(TableHashage *table, char *cle)
{
  unsigned int index;
  unsigned int i = 0;

  /* Convert our string to an integer */
  while(index < ULONG_MAX && i < strlen(cle))
    {
      index = index << 8;
      index += cle[i];
      i++;
    }

  return index % table->size;
}

/* Insert a key-value pair into a hash table. */
void ht_set(TableHashage *hashtable, char *key, char *value) {
  int bin = 0;
  Entree *newpair = NULL;
  Entree *next = NULL;
  Entree *last = NULL;

  bin = convertir_chaine(hashtable, key);

  next = hashtable->vals[ bin ];

  while(next != NULL && next->cle != NULL && strcmp(key, next->cle) > 0) {
      last = next;
      next = next->suiv;
    }

  /* There's already a pair.  Let's replace that string. */
  if(next != NULL && next->cle != NULL && strcmp(key, next->cle) == 0) {

      free(next->value);
      next->value = strdup(value);

      /* Nope, could't find it.  Time to grow a pair. */
    } else {
      newpair = ht_newpair(key, value);

      /* We're at the start of the linked list in this bin. */
      if(next == hashtable->vals[ bin ]) {
          newpair->suiv = next;
          hashtable->vals[ bin ] = newpair;

          /* We're at the end of the linked list in this bin. */
        } else if (next == NULL) {
          last->suiv = newpair;

          /* We're in the middle of the list. */
        } else  {
          newpair->suiv = next;
          last->suiv = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
char *ht_get(TableHashage *hashtable, char *key) {
  int bin = 0;
  Entree *pair;

  bin = convertir_chaine(hashtable, key);

  /* Step through the bin, looking for our value. */
  pair = hashtable->vals[ bin ];
  while(pair != NULL && pair->cle != NULL && strcmp(key, pair->cle) > 0) {
      pair = pair->suiv;
    }

  /* Did we actually find anything? */
  if(pair == NULL || pair->cle == NULL || strcmp(key, pair->cle) != 0) {
      return NULL;

    } else {
      return pair->value;
    }

}


int main()
{
  TableHashage *hashtable = tablehashage_creer(65536);

  ht_set(hashtable, "key1", "inky");
  ht_set(hashtable, "key2", "pinky");
  ht_set(hashtable, "key3", "blinky");
  ht_set(hashtable, "key4", "floyd");

  printf("%s\n", ht_get(hashtable, "key1"));
  printf("%s\n", ht_get(hashtable, "key2"));
  printf("%s\n", ht_get(hashtable, "key3"));
  printf("%s\n", ht_get(hashtable, "key4"));

  return 0;
}
