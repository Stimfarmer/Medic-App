#include "fonction_client_on_serveur.h"
#include "account.h"
#include <stdio.h>

int quit_function( void *sock_fd)
{
   int sock = *(int*)sock_fd;
   char *message;
   message = "Vous allez etre deconnecte...\n";
   
   write((int)sock,message,strlen(message));

   message = "quit";

   write((int)sock,message,strlen(message));
   
   //On libère le pointeur sur le numéro de descripteur de la socket
   free(sock_fd);

   puts("Client déconnecté");
   fflush(stdout);

   return 0;
}

int inscription_function( void *sock_fd)
{
   int sock = *(int*)sock_fd;
   char *message;
 
   message = "Veuillez remplir les champs necessaires a la creation de votre compte\n";

   write((int)sock,message,strlen(message));

   FILE* bdd = NULL;
   bdd = fopen("bdd.txt","a");
   fclose(bdd);

   int  test = creat_account(bdd,sock_fd);

   test = test+1;

   return 0;
}
