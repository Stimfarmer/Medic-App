#include "fonction_serveur.h"
#include "fonction_client_on_serveur.h"
#include "chaine.h"
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>


/*void *connection_handler(void *socket_desc)
{
   //On récupère le numéro de descripteur de la socket
   int sock = *(int*)socket_desc;
   int read_size;
   char *message ,client_message[2000];
   char commande[100];
     
   //On envoie quelques informations au client
   message = "Bonjour, je suis le thread qui te gere!\n1-\"auth\" pour vous authentifier!\n2-\"insc\" pour vous inscrire!\n3-\"help\" pour obtenir de l'aide\n4-\"quit\" pour quitter le serveur!\n";
   write(sock , message , strlen(message));
     
   //On reçoit un message de la part du client
   while( (read_size = recv(sock , client_message , sizeof(client_message) , 0)) > 0 )
   {
      printf("Message provenant du client: %s\n", client_message);

      if((delete_end_char(commande,sizeof(commande),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         break;
      }
      
      printf("Commande provenant du client: %s\n\n", commande);

      function_to_select((void *)socket_desc,commande);

      bzero(client_message,2000);
      bzero(commande,100);
   }
     
   if(read_size == -1)
   {
      perror("Failed to receive");
   }
     
   return 0;
}
*/



int function_to_select(SSL *ssl, char *cmd)
{
   printf("Fonction de choix de la commande\n");

   char *message;
   //int sock = *(int*)socket_desc;
   message = " est votre commande\n";
   
   if(strcmp(cmd,"auth") == 0)
   {
      printf("Authentification\n");
      strcat(cmd,message);
      SSL_write(ssl , cmd , strlen(cmd));
      //authentification_function(socket_desc);
      return 0;  
   }
   else if(strcmp(cmd,"insc") == 0)
   {
      printf("Inscription\n");
      strcat(cmd,message);
      SSL_write(ssl , cmd , strlen(cmd));
      //inscription_function(socket_desc);
      return 0;
   }
   else if(strcmp(cmd,"help") == 0)
   {
      printf("Aide\n");
      strcat(cmd,message);
      SSL_write(ssl , cmd , strlen(cmd));
      //affichage_aide_function(socket_desc);
      return 0;
   }
   else if(strcmp(cmd,"quit") == 0)
   {
      printf("Quitter\n");
      strcat(cmd,message);
      SSL_write(ssl , cmd , strlen(cmd));
      //quit_function(socket_desc);
      return 0;
   }
   else
      printf("Commande non valide\n");
      message = "Entrez une commande valide...\n";
      SSL_write(ssl, message, strlen(message));
      return -1;

}


