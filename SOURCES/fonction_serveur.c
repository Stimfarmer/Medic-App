#include "fonction_serveur.h"
#include "chaine.h"

void *connection_handler(void *socket_desc)
{
   //On récupère le numéro de descripteur de la socket
   int sock = *(int*)socket_desc;
   int read_size;
   char *message ,client_message[2000];
   char commande[100];
     
   //On envoie quelques informations au client
   message = "Bonjour, je suis le thread qui te gere!\n";
   write(sock , message , strlen(message));
     
   message = "Ecrivez quelquechose et je vous renverrai le meme message!\n";
   write(sock , message , strlen(message));
     
   //On reçoit un message de la part du client
   while( (read_size = recv(sock , client_message , sizeof(client_message) , 0)) > 0 )
   {
      printf("Message provenant du client: %s", client_message);

      message = " est votre commande\n";

      if((delete_end_char(commande,sizeof(commande),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         break;
      }
      
      printf("Commande provenant du client: %s\n\n", commande);

      if(strcmp(commande,"quit") == 0)
      {
         puts("Client déconnecté");
         fflush(stdout);
         break;
      }

      strcat(commande,message);

      //Puis on le renvoie au client
      write(sock , commande , strlen(commande));

      bzero(client_message,2000);
      bzero(commande,100);
   }
     
   if(read_size == -1)
   {
      perror("Failed to receive");
   }
         
   //On libère le pointeur sur le numéro de descripteur de la socket
   free(socket_desc);
     
   return 0;
}
