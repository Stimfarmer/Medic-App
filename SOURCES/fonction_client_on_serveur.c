#include "fonction_client_on_serveur.h"
#include "account.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int quit_function( void *ssl)
{
   
   char *message;
   /*message = "Vous allez etre deconnecte...\n";
   
   SSL_write(ssl,message,strlen(message));*/

   message = "quit";

   SSL_write(ssl,message,strlen(message));
   
   //On libère le pointeur sur le numéro de descripteur de la socket
   free(ssl);

   puts("Client déconnecté");
   fflush(stdout);

   return 0;
}

int inscription_function( void *ssl)
{
   //char *message;
 
   /*message = "Veuillez remplir les champs necessaires a la creation de votre compte\nSaisir votre nom\n";

   SSL_write(ssl,message,strlen(message));*/

   FILE* bdd = NULL;
   bdd = fopen("bdd.txt","a");
   fclose(bdd);

   int  test = creat_account(bdd,ssl);
   
   if(test == -1)
   {
      return -1;
   }
   
   return 0;
}

int authentification_function( void *ssl, int*log)
{
   //char *message;
 
   /*message = "Veuillez remplir les champs necessaires a l'authentification\nSaisir votre login\n";

   SSL_write(ssl,message,strlen(message));*/

   user usr;
   FILE* bdd = NULL;
   bdd = fopen("bdd.txt","a");
   fclose(bdd);

   inlog(usr,bdd,ssl,log);


   return 0;
}

int deauth_function(void *ssl, int*log)
{
	*(log) = 0;
	return 0;
}

