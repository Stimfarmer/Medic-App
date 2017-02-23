#include "fonction_client_on_serveur.h"
#include <stdio.h>

int quit_function( void *sock_fd)
{
   int sock = *(int*)sock_fd;
   char *message;
   message = "Vous allez etre deconnecte...\n";
   
   write((int)sock,message,strlen(message));   
   
   //On libère le pointeur sur le numéro de descripteur de la socket
   free(sock_fd);

   puts("Client déconnecté");
   fflush(stdout);

   return 0;
}
