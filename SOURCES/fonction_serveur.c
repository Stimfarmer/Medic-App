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
   printf("Fonction de choix de la commande IN\n");

   char *message;

   char cat[2048];
   char *cmd_f;
   FILE *to_send;
   char buf[1024];
   char commande_f[50];
   int error;
  

   message = " est votre commande\n";
   strcpy(commande_f ,cmd);
   
   if(strcmp(cmd,"") == 0)
   {
      message = "Aucune commande saisie.\nEntrez une commande valide.\n";
      SSL_write(ssl , message , strlen(message));
      printf("Fonction de choix de la commande OUT\n");
      return -1;
   }

   if(strcmp(cmd,"auth") == 0)
   {
      printf("Authentification IN\n");
      strcat(cmd,message);

      SSL_write(ssl , cmd , strlen(cmd));
      //authentification_function(socket_desc);

      authentification_function(ssl);
      printf("Authentification OUT\n");
      printf("Fonction de choix de la commande OUT\n");

      return 0;  
   }
   else if(strcmp(cmd,"insc") == 0)
   {
      printf("Inscription IN\n");
      strcat(cmd,message);

      SSL_write(ssl , cmd , strlen(cmd));
      //inscription_function(socket_desc);


      error = inscription_function(ssl);
      if(error == -1)
      {
         message = "Inscription impossible, compte existant!\n";
         SSL_write(ssl , message , strlen(message));
         printf("Inscription OUT\n");
         printf("Fonction de choix de la commande OUT\n");
         return -1;
      }
      message = "Inscription realisee avec succes!\n";
      SSL_write(ssl , message , strlen(message));
      printf("Inscription OUT\n");
      printf("Fonction de choix de la commande OUT\n");

      return 0;
   }
   else if(strcmp(cmd,"help") == 0)
   {
      printf("Aide IN\n");
      strcat(cmd,message);

      SSL_write(ssl , cmd , strlen(cmd));


      message = "1-\"auth\" pour vous authentifier!\n2-\"insc\" pour vous inscrire!\n3-\"help\" pour obtenir de l'aide\n4-\"quit\" pour quitter le serveur!\n";
      SSL_write(ssl , message , strlen(message));

      //affichage_aide_function(socket_desc);
      printf("Aide OUT\n");
      printf("Fonction de choix de la commande OUT\n");
      return 0;
   }
   else if(strcmp(cmd,"quit") == 0)
   {
      printf("Quitter IN\n");
      strcat(cmd,message);

      SSL_write(ssl , cmd , strlen(cmd));
      quit_function(ssl);
      printf("Quitter OUT\n");
      printf("Fonction de choix de la commande OUT\n");
      return 0;
   }
   else
      cmd_f = strtok(cmd," ");

      if(strcmp(cmd_f,"ls") == 0)
      {
         printf("Ls serveur\n");
         //system(commande_f);
         if (( to_send = popen(commande_f, "r")) == NULL)
         {
            perror("popen");
            exit(1);
         }
         while(fgets(buf, sizeof(buf), to_send))
         {
            strcat(cat,buf);
            
         }
         SSL_write(ssl , cat , strlen(cat));
         
         pclose(to_send);
         
      }
      else if(strcmp(cmd_f,"mkdir") == 0)
      {
         printf("Mkdir serveur\n");
         message = "Fichier créé\n";
         system(commande_f);
         SSL_write(ssl , message , strlen(message));
      }
      else if(strcmp(cmd_f,"cd") == 0)
      {
         printf("Cd serveur\n");
         system(commande_f);
      }
      else if(strcmp(cmd_f,"rm") == 0)
      {
         printf("Rm serveur\n");
         system(commande_f);
      }
      else if(strcmp(cmd_f,"cp") == 0)
      {
         printf("Cp serveur\n");
         system(commande_f);
      }
      else if(strcmp(cmd_f,"touch") == 0)
      {
         printf("Touch serveur\n");
         system(commande_f);
      }
      else if(strcmp(cmd_f,"vim") == 0)
      {
         printf("Vim serveur\n");
         system(commande_f);
      }
      strcat(commande_f,message);
      SSL_write(ssl , commande_f , strlen(commande_f));
      printf("Fonction de choix de la commande OUT\n");
      bzero(cat,2048);
      bzero(buf,1024);
      bzero(commande_f,50);

      return -1;

}

