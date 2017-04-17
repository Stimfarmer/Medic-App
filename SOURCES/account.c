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

void generate_passwd(user usr)
{
   char list[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0";
   char *password = malloc(20*sizeof(char));
   int i;
   
   int a = 0;
   int b = 51;
   int nb;
   srand(time(NULL));
   
 
   for(i = 0 ; i < 9 ; i++)
   {
      nb = rand()%(b-a) +a;
      password[i] = list[nb];
   }
   password[10] = '\0';
   bzero(usr.mdp,20);
   sprintf(usr.mdp,password);
   free(password);
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
   bdd = fopen("SOURCES/bdd.txt","r");
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

void search_log(char *log,int number,char *stck)
{
	FILE *bdd = NULL;
   bdd = fopen("SOURCES/bdd.txt","r");
  
  
   int dust1;
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *search = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));
   char *dust6 = malloc(20*sizeof(char));
   char *mot = malloc(20*sizeof(char));
  
   int i=0;
   int j;
   int cursor;
  
   while((cursor = fgetc(bdd)) != EOF)
   {
		fscanf(bdd,"%d %s %s %s %s %s %s",&dust1,dust2,dust3,dust4,search,dust5,dust6);
  
		if(strcmp(search,log) == 0)
		{
         i++;
      }
   }
  
   if(i != 0)
   {
      number++;
      bzero(log,strlen(log));
      for(j=0;j<strlen(stck);j++)
      {
         log[j] = stck[j];
      }
		printf("AVANT:%s\n",log); 
      sprintf(mot,"%d",number);
      strcat(log,mot);
		printf("[%s]\n",log);
      search_log(log,number,stck);
   }
	free(dust2);
	free(dust3);
	free(dust4);
	free(search);
	free(dust5);
	free(dust6);
	free(mot);


}






int ask(user usr, void *ssl)
{
   char *message ,client_message[2000],msg[100];
   int read_size;
   int recup_result_chaine;
   
   do 
   {
      message = "Veuillez remplir les champs necessaires a la creation de votre compte\nSaisir votre nom\n";

      SSL_write(ssl,message,strlen(message));

     
      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }
      read_size=strlen(msg);
     
      recup_result_chaine=is_alpha(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup result chaine: %i\n",recup_result_chaine);

      strcpy(usr.name,msg);

      printf("Name:%s\n",usr.name);  

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));

   do 
   {
      message = "Saisir votre surname\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }
      read_size=strlen(msg);

      recup_result_chaine=is_alpha(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup result chaine: %i\n",recup_result_chaine);

      strcpy(usr.surname,msg);

      printf("Surname:%s\n",usr.surname);  

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));

   do 
   {
      message = "Saisir votre fonction\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }
      read_size=strlen(msg);

      recup_result_chaine=is_alpha(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup result chaine: %i\n",recup_result_chaine);

      strcpy(usr.fonction,msg);


      if(strcmp(usr.fonction,"medecin")==0)
      {
         usr.droit = 'm';
      }
      if(strcmp(usr.fonction,"root")==0)
      {
         usr.droit = 'r';
      }
      if(strcmp(usr.fonction,"infirmiere")==0)
      {
         usr.droit = 'i';
      }

      printf("Fonction:%s\n",usr.fonction);

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));

   do 
   {
      message = "Saisir votre numero de secu\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }
      read_size=strlen(msg);

      recup_result_chaine=is_num(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup result chaine: %i\n",recup_result_chaine);

      usr.nb_secu = atoi(msg);

      printf("Nb_secu:%i\n",usr.nb_secu);

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));
/*
   do 
   {
      message = "Saisir votre mdp ( mdp alpha_numérique )\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         return -1;
      }
      read_size=strlen(msg);
      recup_result_chaine=is_alnum(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup_result_chaine: %i\n",recup_result_chaine);

      strcpy(usr.mdp,msg);

      printf("Mdp:%s\n",usr.mdp);  

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));*/


   generate_passwd(usr);
   printf("MDP:%s",usr.mdp);
	
	bzero(usr.login,20);
      if(usr.surname[0]>64 && usr.surname[0]<91)
   {   
      usr.login[0] = usr.surname[0]+32;
   }
   if(usr.surname[0]>96 && usr.surname[0]<123)
   {
      usr.login[0] = usr.surname[0];
   }

   int i;
   for(i=1;i<=strlen(usr.name);i++)
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
	char *stck = malloc(20*sizeof(char));
	int j;
	bzero(stck,20);
	for(j=0;j<strlen(usr.login);j++)
	{
		stck[j] = usr.login[j];
	}
	printf("-LA----%s-----\n",usr.login);
	search_log(usr.login,0,stck);
	printf("FINAL:%s\n",usr.login);
	free(stck);
		

   read_size++;
   return usr.nb_secu;
}



int inlog(user usr,FILE* bdd, void* ssl, int*login)
{
   char *log = malloc(20*sizeof(char));
   char *mdp = malloc(20*sizeof(char));
   char *message ,client_message[2000],msg[100],petit_buffer[50];
   int read_size;
	
	time_t rawtime;
   //struct tm *info;
   time( &rawtime );
   //info = localtime( &rawtime );


	FILE *log_history = NULL;
	log_history = fopen("SOURCES/log.txt","a");
	fscanf(log_history,"ntm");
 
   usr = alloc_user(usr);
 
   int cursor;
   bdd = fopen("SOURCES/bdd.txt","r");
   
   do
   {
      message = "Veuillez remplir les champs necessaires a l'authentification\nSaisir votre login\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }

   read_size=strlen(msg);

   printf("Read size: %i\n",read_size);

   strcpy(usr.login,msg);

   printf("Login:%s\n",usr.login);
 

   bzero(client_message,2000);
   bzero(msg,100);
   }while( read_size <= 0);

   do
   {
   message = "Saisir votre mdp\n";

   SSL_write(ssl,message,strlen(message));

   read_size = SSL_read(ssl , client_message , sizeof(client_message));

   if((delete_end_char(msg,sizeof(msg),client_message))==-1)
   {
      perror("Erreur supression caractère de fin!");
      return -1;
   }
      read_size=strlen(msg);

      printf("Read size: %i\n",read_size);

      strcpy(usr.mdp,msg);

      printf("Mdp:%s\n",usr.mdp);

      bzero(client_message,2000);
      bzero(msg,100);
   }while( read_size <= 0);


   while((cursor =fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%d %s %s %s %s %s %s",&usr.nb_secu,usr.name,usr.surname,usr.fonction,log,mdp,usr.year);
      if(strcmp(log,usr.login)==0 && strcmp(mdp,usr.mdp) == 0)
      {
      	printf("\nLOG IN SUCCESS\n");
      	message = "\nLog as ";

         if(strcmp(usr.fonction,"medecin")==0)
         {
            usr.droit = 'm';
         }
         if(strcmp(usr.fonction,"root")==0)
         {
            usr.droit = 'r';
         }
         if(strcmp(usr.fonction,"infirmiere")==0)
         {
            usr.droit = 'i';
         }

      	bzero(petit_buffer,50);
      	sprintf(petit_buffer,message);
      	sprintf(petit_buffer + strlen(petit_buffer),usr.login);
      	sprintf(petit_buffer + strlen(petit_buffer),"\n");
      	*(login) = 1;
      	SSL_write(ssl,petit_buffer,strlen(petit_buffer));
      	fclose(log_history);
      	return 0;
      }
   }
/*
	fscanf(log_history,"%s %d:%d:%d %d/%d/%d\n",usr.login,&info->tm_sec,&info->tm_min,&info->tm_hour,&info->tm_mday,&info->tm_mon+1,&info->tm_year+1900);
	fscanf(log_history,"tamere");*/

   printf("Login:%s\n",usr.login);
   printf("Mdp registered:%s\n",usr.mdp);
   printf("Mdp fscanf:%s\n",mdp);

   printf("\nERREUR MDP OR LOG\n");
   message = "\nERREUR MDP OR LOG\n";

   SSL_write(ssl,message,strlen(message));
   fclose(bdd);
   desalloc_user(usr);
   free(log);
   free(mdp);

   read_size++;
   return -1;

}





int creat_account(FILE*bdd, void* ssl)
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

   nb_secu = ask(usr, ssl);

   bdd = fopen("SOURCES/bdd.txt","r");
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
    
   int ret1 = remove("SOURCES/bdd.txt");
   if(ret1 != 0)
   {
      printf("ERREUR: Suppression fichier creat\n");
   }
   
   int ret2 = rename("tmp.txt","SOURCES/bdd.txt");
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



void delete_account(void *ssl,FILE* bdd)
{

   int cursor = 0;
   int verif = 0;
   int cmp = 0;
   int check = 0;
   int nb_secu;
   char *result = malloc(20*sizeof(char));
   int nb_lignes = nb_lines(bdd);

   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));
   char *dust6 = malloc(20*sizeof(char));

   char *message ,client_message[2000],msg[100];
   int read_size;
   int recup_result_chaine;
   
   bdd = fopen("SOURCES/bdd.txt","r");
   FILE * tmp = NULL;
   tmp = fopen("tmp.txt","a");

   fseek(tmp,0,SEEK_SET);
   fseek(bdd,0,SEEK_SET);

   do 
   {
      message = "Numéro de sécu du compte a delete\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         break;
      }
      read_size=strlen(msg);
      recup_result_chaine=is_alnum(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup_result_chaine: %i\n",recup_result_chaine);

      strcpy(result,msg);

      printf("Mdp:%s\n",result);  

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));

   nb_secu = strtol(result,NULL,10);
   printf("nb_secu:%d",nb_secu);
   while((cursor = fgetc(bdd)) != EOF)
   {  
      fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);

      if(verif == nb_secu)
      {
	    printf("\ncompte supprime\n");
	    check = 1;
      }
      else
      {
         if(cmp <nb_lignes )
         {
            fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5,dust6);
         }
      }
      cmp++;
   }

   if(check ==0)
   {
      printf("Compte inexistant\n");
   }


   int ret1 = remove("SOURCES/bdd.txt");
   if(ret1 !=0)
   {
      printf("\nERREUR: suppression fichier delete\n");
   }
   
   int ret2 = rename("tmp.txt","SOURCES/bdd.txt");
   if(ret2 != 0)
   {
      printf("\nERREUR:changement nom fichier delete\n");
   }

   free(dust1);
   free(dust2);
   free(dust3);
   free(dust4);
   free(dust5);
  // free(message);
   fclose(tmp);
   fclose(bdd);
}




int change_mdp(FILE* bdd,void *ssl)
{
   int verif;
   int cmp=0;

   char *old_passwd = malloc(20*sizeof(char));
   char *new_passwd = malloc(20*sizeof(char));
   char *login = malloc(20*sizeof(char));
   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));
   char *dust6 = malloc(20*sizeof(char));
   
   int nb_ligne = nb_lines(bdd);
   bdd = fopen("SOURCES/bdd.txt","r");
   FILE* tmp = NULL;
   tmp = fopen("SOURCES/tmp.txt","a");
   int cursor;

   char *message ,client_message[2000],msg[100];
   int read_size;
   int recup_result_chaine;

   do 
   {
      message = "Enter login\n";

      SSL_write(ssl,message,strlen(message));

      read_size = SSL_read(ssl , client_message , sizeof(client_message));

      if((delete_end_char(msg,sizeof(msg),client_message))==-1)
      {
         perror("Erreur supression caractère de fin!");
         break;
      }
      read_size=strlen(msg);
      recup_result_chaine=is_alnum(msg);

      printf("Read size: %i\n",read_size);
      printf("Recup_result_chaine: %i\n",recup_result_chaine);

      strcpy(login,msg);  

      bzero(client_message,2000);
      bzero(msg,100);
   }while((read_size <= 0) || (recup_result_chaine == -1));

      

   while((cursor = fgetc(bdd)) != EOF)
   {
      if(cmp < nb_ligne)
      {
         fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);
         if(strcmp(dust4,login)==0)
         {
            printf("Enter old password\n");


            do 
            {
               message = "Enter old password\n";

               SSL_write(ssl,message,strlen(message));

               read_size = SSL_read(ssl , client_message , sizeof(client_message));

               if((delete_end_char(msg,sizeof(msg),client_message))==-1)
               {
                  perror("Erreur supression caractère de fin!");
                  break;
               }
               read_size=strlen(msg);
               recup_result_chaine=is_alnum(msg);

               printf("Read size: %i\n",read_size);
               printf("Recup_result_chaine: %i\n",recup_result_chaine);

               strcpy(old_passwd,msg);  

               bzero(client_message,2000);
               bzero(msg,100);
            }while((read_size <= 0) || (recup_result_chaine == -1));

            if(strcmp(old_passwd,dust5)==0)
            {
               printf("New password\n");

               do 
               {
                  message = "Enter new password\n";

                  SSL_write(ssl,message,strlen(message));

                  read_size = SSL_read(ssl , client_message , sizeof(client_message));

                  if((delete_end_char(msg,sizeof(msg),client_message))==-1)
                  {
                     perror("Erreur supression caractère de fin!");
                     break;
                  }
                  read_size=strlen(msg);
                  recup_result_chaine=is_alnum(msg);

                  printf("Read size: %i\n",read_size);
                  printf("Recup_result_chaine: %i\n",recup_result_chaine);

                  strcpy(new_passwd,msg);

           
                  bzero(client_message,2000);
                  bzero(msg,100);
               }while((read_size <= 0) || (recup_result_chaine == -1));

             //  bzero(login,20);
              // sprintf(usr.mdp,new_passwd);
            }
            else
            {
               printf("ERROR: wrong password");
               return -1;
            }
         }
         cmp++;
      }
   }
   fseek(bdd,0,SEEK_SET);
   cmp = 0;

   printf("STAGE 1\n");

   while((cursor = fgetc(bdd)) != EOF)
   {
      if(cmp < nb_ligne)
      {
         fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);
         if(strcmp(dust4,login)==0)
         {
            fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,new_passwd,dust6);
         }
         else
         {
            fprintf(tmp," %d %s %s %s %s %s %s\n",verif,dust1,dust2,dust3,dust4,dust5,dust6);   
         }
         cmp++;
      }
         
   }
   printf("STAGE 2\n");

   int ret1 = remove("SOURCES/bdd.txt");
   if(ret1 != 0)
   {
      printf("ERREUR: Suppression fonction change_log\n");
      return -1;
   }
   int ret2 = rename("SOURCES/tmp.txt","SOURCES/bdd.txt");
   if(ret2 != 0)
   {
      printf("ERREUR: Rename fonction change_log\n");
      return -1;
   }
      

   free(dust1);
   free(dust2);
   free(dust3);
   free(dust4);
   free(dust5);
   free(dust6);
   free(new_passwd);
   free(old_passwd);
  // free(message);
   free(login);
   fclose(bdd);
   fclose(tmp);   
   return 0;

}

