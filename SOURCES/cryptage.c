#include "cryptage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int crypt_simple(char *file_name)
{
   int index;
   int taille_fichier = 0;
   FILE * fichier_avant = NULL;
   FILE * fichier_apres = NULL;
   int taille_memoire_tampon = 1024;
   char * memoire_tampon = NULL;
   // allocation
   memoire_tampon = (char*)malloc(taille_memoire_tampon);
   // mise en mémoire tampon
   fichier_avant = fopen( file_name, "r" );
   taille_fichier = fread( memoire_tampon, 1,taille_memoire_tampon, fichier_avant );
   fclose( fichier_avant );
   // chiffrement dans le mémoire tampon
   for( index = 0; index != taille_fichier; ++index )
   {
      // chiffrement des majuscules
      if( memoire_tampon[ index ] >=65 && memoire_tampon[ index ] < 91 )
      memoire_tampon[ index ] = ( ( memoire_tampon[ index ] - 65 ) + 1 ) % 26 + 65;
      // chiffrement des minuscules
      if( memoire_tampon[ index ] >=97 && memoire_tampon[ index ] < 123 )
      memoire_tampon[ index ] = ( ( memoire_tampon[ index ] - 97 ) + 1 ) % 26 + 97;
   }
   // mise a jour du fichier
   fichier_apres = fopen( file_name, "w" );
   fwrite( memoire_tampon, 1, taille_fichier, fichier_apres );
   fclose( fichier_apres );
   // desallocation
   free( memoire_tampon);

   return 1;
}

int uncrypt_simple(char *file_name)
{
   int index;
   int taille_fichier = 0;
   FILE * fichier_avant = NULL;
   FILE * fichier_apres = NULL;
   int taille_memoire_tampon = 1024;
   char * memoire_tampon = NULL;
   // allocation
   memoire_tampon = (char*)malloc(taille_memoire_tampon);
   // mise en mémoire tampon
   fichier_avant = fopen( file_name, "r" );
   taille_fichier = fread( memoire_tampon, 1,taille_memoire_tampon, fichier_avant );
   fclose( fichier_avant );
   // chiffrement dans le mémoire tampon
   for( index = 0; index != taille_fichier; ++index )
   {
      // chiffrement des majuscules
      if( memoire_tampon[ index ] >=65 && memoire_tampon[ index ] < 91 )
      memoire_tampon[ index ] = ( ( memoire_tampon[ index ] - 65 ) - 1 ) % 26 + 65;
      // chiffrement des minuscules
      if( memoire_tampon[ index ] >=97 && memoire_tampon[ index ] < 123 )
      memoire_tampon[ index ] = ( ( memoire_tampon[ index ] - 97 ) - 1 ) % 26 + 97;
   }
   // mise a jour du fichier
   fichier_apres = fopen( file_name, "w" );
   fwrite( memoire_tampon, 1, taille_fichier, fichier_apres );
   fclose( fichier_apres );
   // desallocation
   free( memoire_tampon);
   return 1;

}
