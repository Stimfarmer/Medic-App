#include "account.h"
#include "chaine.h"
/**
* \file account.c
* \author JJaouen
* \version 1.0
* \date 07/03/2017
* \brief Fonctions relatives à l'administration de compte
*/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/**
* \brief Fonction initialisant un user
* \param usr  user a initialiser
* \return retourne le user initialisé
*/
void init_user(user usr)
{
   usr.name = "Antoine";
   usr.fonction = "Medecin";
   usr.nb_secu = 9999;
   usr.mdp = "azerty";
   usr.surname = "Marchand";
   usr.login = "amarchan";
   printf("%s %s %d %s\n",usr.name,usr.fonction,usr.nb_secu,usr.mdp);
}
 
/**
* \brief Fonction allouant les char* de la structure user
* \param usr user a allouer
* \return retourne un user
*/  
user alloc_user(user usr)
{
   usr.name = malloc(20*sizeof(char));
   usr.surname = malloc(20*sizeof(char));
   usr.fonction = malloc(20*sizeof(char));
   usr.mdp = malloc(20*sizeof(char));
   usr.login = malloc(20*sizeof(char));
   usr.year = malloc(20*sizeof(char));
   return usr;
}

/**
* \brief Fonction desallouant un user
* \param usr user a desallouer
* \return ne renvoie rien
*/
void desalloc_user(user usr)
{
   free(usr.name);
   free(usr.fonction);
   free(usr.mdp);
   free(usr.surname);
   free(usr.login);
   free(usr.year);
}


int nb_lines(FILE* bdd)
{
   int cursor = 0;
   int nb_lignes = 0;
   bdd = fopen("bdd.txt","r");
   if(bdd != NULL)
   {
      while ((cursor = getc(bdd)) != EOF)
      {
	 if(cursor == 10)
	 {
	    ++nb_lignes;
	 }
      }
   }
   else
   {
      printf("ERREUR:Ouverture fichier nb_lines\n");
   }
   
   fseek(bdd,0,SEEK_SET);
   printf("nblignes: %d\n",nb_lignes);
   fclose(bdd);
   return nb_lignes;
}

int ask(user usr, void *sock_fd)
{
   char *message ,client_message[2000],msg[100];
   int sock = *(int*)sock_fd;
   int read_size;
   
   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.name,msg);

   printf("Name:%s\n",usr.name);  

   bzero(client_message,2000);
   bzero(msg,100);

   message = "Saisir votre surname\n";

   write((int)sock,message,strlen(message));

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.surname,msg);

   printf("Surname:%s\n",usr.surname);  

   bzero(client_message,2000);
   bzero(msg,100);

   message = "Saisir votre fonction\n";

   write((int)sock,message,strlen(message));

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.fonction,msg);

   printf("Fonction:%s\n",usr.fonction);

   bzero(client_message,2000);
   bzero(msg,100);

   message = "Saisir votre numero de secu\n";

   write((int)sock,message,strlen(message));

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }

   usr.nb_secu = atoi(msg);

   printf("Nb_secu:%i\n",usr.nb_secu);

   bzero(client_message,2000);
   bzero(msg,100);

   message = "Saisir votre mdp\n";

   write((int)sock,message,strlen(message));

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.mdp,msg);

   printf("Mdp:%s\n",usr.mdp);  

   bzero(client_message,2000);
   bzero(msg,100);

      if(usr.surname[0]>64 && usr.surname[0]<91)
   {   
      usr.login[0] = usr.surname[0]+32;
   }
   if(usr.surname[0]>96 && usr.surname[0]<123)
   {
      usr.login[0] = usr.surname[0];
   }

   int i;
   for(i=1;i<=7;i++)
   {
      if(usr.name[i-1]>64 && usr.name[i-1]<91)
      {
	 usr.login[i] = usr.name[i-1]+32;
      }
      if(usr.name[i-1]>96 && usr.name[i-1]<123)
      {
	 usr.login[i] = usr.name[i-1];
      }
   }

   read_size++;

   return usr.nb_secu;
}



int inlog(user usr,FILE* bdd, void* sock_fd)
{
   char *log = malloc(20*sizeof(char));
   char *mdp = malloc(20*sizeof(char));
   int sock = *(int*)sock_fd;
   //char rep[200];
   char *message ,client_message[2000],msg[100];
   int read_size;
 
   usr = alloc_user(usr);
 
   int cursor;
   bdd = fopen("bdd.txt","r");

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.login,msg);

   printf("Login:%s\n",usr.login);  

   bzero(client_message,2000);
   bzero(msg,100);

   message = "Saisir votre mdp\n";

   write((int)sock,message,strlen(message));

   read_size = recv(sock , client_message , sizeof(client_message) , 0);

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   strcpy(usr.mdp,msg);

   printf("Mdp:%s\n",usr.mdp);

   bzero(client_message,2000);
   bzero(msg,100);

   /*printf("\nLogin:");
   scanf("%s",usr.login);
   printf("\nMdp:");
   scanf("%s",usr.mdp);
   */
   

   while((cursor =fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%d %s %s %s %s %s %s",&usr.nb_secu,usr.name,usr.surname,usr.fonction,log,mdp,usr.year);
      if(strcmp(log,usr.login)==0 && strcmp(mdp,usr.mdp) == 0)
      {
	 printf("\nLOG IN SUCCESS\n");
	 message = "\nLog as ";
         write((int)sock,message,strlen(message));
         write((int)sock,usr.login,strlen(usr.login));
	 message = "\n";
         write((int)sock,message,strlen(message));
	 return 0;
      }
   }

   printf("\nERREUR MDP OR LOG\n");
   message = "\nERREUR MDP OR LOG\n";

   write((int)sock,message,strlen(message));
   fclose(bdd);
   desalloc_user(usr);
   free(log);
   free(mdp);

   read_size++;
   return -1;

}





int creat_account(FILE*bdd, void* sock_fd)
{
   int cursor;
   int trouve = 0;
   int verif=0;

   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));
   char *dust6 = malloc(20*sizeof(char));

   user usr;
   usr = alloc_user(usr);

   int nb_secu = 0;
   int cmp=1;
   int stck = 0;


   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );


   FILE *tmp = NULL;
   tmp = fopen("tmp.txt","a");

   int nb_lignes = nb_lines(bdd);

   //printf("\n______CREATION DE COMPTE______\n");

   nb_secu = ask(usr, sock_fd);

   bdd = fopen("bdd.txt","r");
   while((cursor = fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);

      if(verif == nb_secu)
      {
         trouve = 1;
         printf("Compte existant\n");
         return -1;
      }
   }

   if(trouve == 0)
   {
      fseek(bdd,0,SEEK_SET);
      if(nb_lignes ==0)
      {
         fprintf(tmp," %d %s %s %s %s %s %d/%d/%d\n",nb_secu,usr.name,usr.surname,usr.fonction,usr.login,usr.mdp,info->tm_mday,info->tm_mon+1,info->tm_year+1900);
      }
      else
      {
         while((cursor = fgetc(bdd)) != EOF )
         { 
            fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);
            if(verif < nb_secu)
            {
               fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5,dust6);
               if(cmp == nb_lignes)
               {
                  fprintf(tmp," %d %s %s %s %s %s %d/%d/%d\n",nb_secu,usr.name,usr.surname,usr.fonction,usr.login,usr.mdp,info->tm_mday,info->tm_mon+1,info->tm_year+1900);
                  break;
               }
            }
            if(verif > nb_secu)
            {
               if(stck == 0)
               {
                  fprintf(tmp," %d %s %s %s %s %s %d/%d/%d\n",nb_secu,usr.name,usr.surname,usr.fonction,usr.login,usr.mdp,info->tm_mday,info->tm_mon+1,info->tm_year+1900);
                  fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5,dust6);
               }
               if(stck != 0 && cmp < nb_lignes +1)
               {
                  fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5,dust6);
               }
               stck = 1;
            }
            cmp++;	     
         }
      }
   }
    
   int ret1 = remove("bdd.txt");
   if(ret1 != 0)
   {
      printf("ERREUR: Suppression fichier creat\n");
   }
   
   int ret2 = rename("tmp.txt","bdd.txt");
   if(ret2 != 0)
   {
      printf("ERREUR: Rename fichier creat\n");
   }
   
   fclose(tmp);
   fclose(bdd);
   free(dust1);
   free(dust2);
   free(dust3);
   free(dust4);
   free(dust5);
   desalloc_user(usr);
   
   return 0; 
}



void delete_account(int nb_secu,FILE* bdd)
{
   int cursor = 0;
   int verif = 0;
   int cmp = 0;
   int check = 0;
   int nb_lignes = nb_lines(bdd);

   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));

   bdd = fopen("bdd.txt","r");
   FILE * tmp = NULL;
   tmp = fopen("tmp.txt","a");

   fseek(tmp,0,SEEK_SET);
   fseek(bdd,0,SEEK_SET);

   while((cursor = fgetc(bdd)) != EOF)
   {  
      fscanf(bdd,"%d %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5);

      if(verif == nb_secu)
      {
	    printf("\ncompte supprime\n");
	    check = 1;
      }
      else
      {
         if(cmp <nb_lignes )
         {
            fprintf(tmp," %d %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5);
         }
      }
      cmp++;
   }

   if(check ==0)
   {
      printf("Compte inexistant\n");
   }


   int ret1 = remove("bdd.txt");
   if(ret1 !=0)
   {
      printf("\nERREUR: suppression fichier delete\n");
   }
   
   int ret2 = rename("tmp.txt","bdd.txt");
   if(ret2 != 0)
   {
      printf("\nERREUR:changement nom fichier delete\n");
   }

   free(dust1);
   free(dust2);
   free(dust3);
   free(dust4);
   free(dust5);
   fclose(tmp);
   fclose(bdd);
}
