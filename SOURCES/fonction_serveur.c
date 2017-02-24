#include "fonction_serveur.h"
#include "fonction_client_on_serveur.h"
#include "chaine.h"
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>






int function_to_select(SSL *ssl, char *cmd)
{
   printf("Fonction de choix de la commande IN\n");

   char *message;
   int ret;
   char cat[2048];
   char *cmd_f;
   FILE *to_send;
   char buf[1024];
   char commande_f[50];
   int error;
   printf("La cmd est %s\n",cmd);
  

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
      authentification_function(ssl);
      printf("Authentification OUT\n");
      printf("Fonction de choix de la commande OUT\n");

      return 0;  
   }
   else if(strcmp(cmd,"insc") == 0)
   {
      printf("Inscription IN\n");
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
      message = "Bonjour!\n1- auth pour authentifier\n2- insc pour inscrire\n3- help pour afficher l'aide\n4- quit pour quitter le serveur\nBonne navigation\n";
      SSL_write(ssl , message , strlen(message));
      printf("Aide OUT\n");
      printf("Fonction de choix de la commande OUT\n");
      return 0;
   }
   else if(strcmp(cmd,"quit") == 0)
   {
      printf("Quitter IN\n");
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
	 bzero(cat,2048);
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
         system(commande_f);
	 message = "Fichier créé\n";
         SSL_write(ssl , message , strlen(message));
      }
      else if(strcmp(cmd_f,"cd") == 0)
      {
         printf("Cd serveur\n");
         system(commande_f);
	 message = "Cd effectué\n";
         SSL_write(ssl , message , strlen(message));
      }
      else if(strcmp(cmd_f,"rm") == 0)
      {
         printf("Rm serveur\n");
         ret = system(commande_f);
         if(ret == -1){
            perror("Erreur rm");
	    return EXIT_FAILURE;
         }
	 else
         {
            message = "Fichier supprimé\n";
            SSL_write(ssl , message , strlen(message));
      	 }
      }
      else if(strcmp(cmd_f,"cp") == 0)
      {
         printf("Cp serveur\n");
         system(commande_f);
	 message = "Copie effectuée\n";
         SSL_write(ssl , message , strlen(message));
      }
      else if(strcmp(cmd_f,"touch") == 0)
      {
         printf("Touch serveur\n");
         system(commande_f);
	 message = "Fichier créé\n";
         SSL_write(ssl , message , strlen(message));
      }
      else if(strcmp(cmd_f,"vim") == 0)
      {
         printf("Vim serveur\n");
         system(commande_f);
	 message = "Vim lancé\n";
         SSL_write(ssl , message , strlen(message));
      }
      else
      {
         message = "Entrez une commande valide.\n";
         SSL_write(ssl , message , strlen(message));
      }
      printf("Fonction de choix de la commande OUT\n");
      bzero(cat,2048);
      bzero(buf,1024);
      bzero(commande_f,50);

      return -1;

}


