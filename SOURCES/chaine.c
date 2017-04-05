#include "chaine.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int is_alpha( char* chaine)
{
   int i=0;

   while (chaine[i] != '\0')
   {
      if(!isalpha(chaine[i]))
      {
         return -1;
      }
      i++;
   }

   return 1;
}

int is_alnum( char* chaine)
{
   int i=0;

   while (chaine[i] != '\0')
   {
      if(!isalnum(chaine[i]))
      {
         return -1;
      }
      i++;
   }

   return 1;
}

int is_num( char* chaine)
{
   int i=0;

   while (chaine[i] != '\0')
   {
      if(!isdigit(chaine[i]))
      {
         return -1;
      }
      i++;
   }

   return 1;
}

int delete_end_char( char *chaine_sortie, int taille_sortie, char *chaine_entree)
{
   char const *chaine_tmp_start, *chaine_tmp_end;
   unsigned int taille;
   char *chaine_tmp_sortie;

    
   if (0 == taille_sortie || !chaine_entree || !chaine_sortie)
      return -1;

   chaine_tmp_start = chaine_entree;
   chaine_tmp_end = chaine_entree + strlen(chaine_entree) - 1;

    
   while (*chaine_tmp_start && isspace(*chaine_tmp_start))
      ++chaine_tmp_start;

    
   while (chaine_tmp_end >= chaine_entree && isspace(*chaine_tmp_end))
      --chaine_tmp_end;

   chaine_tmp_sortie = chaine_sortie;
   taille = 0;

    
   while (chaine_tmp_start <= chaine_tmp_end && taille < taille_sortie - 1)
   {
      *chaine_tmp_sortie++ = *chaine_tmp_start++;
      ++taille;
   }

   *chaine_tmp_sortie = '\0';
   return taille;
}
