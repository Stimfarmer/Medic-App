#include "fonction_serveur.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *socket_desc)
{
   //On récupère le numéro de descripteur de la socket
   int sock = *(int*)socket_desc;
   int read_size;
   char *message , client_message[2000];
     
   //On envoie quelques informations au client
   message = "Bonjour, je suis celui qui te gere\n";
   write(sock , message , strlen(message));
     
   message = "Ecrivez quelquechose et je vous renverrai le meme message!\n";
   write(sock , message , strlen(message));
     
   //On reçoit un message de la part du client
   while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
   {
       //Puis on le renvoie au client
       write(sock , client_message , strlen(client_message));
   }
     
   if(read_size == 0)
   {
       puts("Client déconnecté");
       fflush(stdout);
   }
   else if(read_size == -1)
   {
       perror("Failed to receive");
   }
         
   //On libère le pointeur sur le numéro de descripteur de la socket
   free(socket_desc);
     
   return 0;
}
