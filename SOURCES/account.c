<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
   char *name;
   char *fonction;
   int nb_secu;
   char *droit;
   char *mdp;
} user;
=======
#include "account.h"
#include "chaine.h"
>>>>>>> dde80e0c9f0efb3f65c152835b84b5fb18ac6eae

void init_user(user usr)
{
   usr.name = "Antoine";
   usr.fonction = "Medecin";
   usr.nb_secu = 9999;
   usr.droit  = "A";
   usr.mdp = "azerty";
   printf("%s %s %d %s\n",usr.name,usr.fonction,usr.nb_secu,usr.mdp);
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

<<<<<<< HEAD
int ask(char *name, char* fonction, int nb_secu)
{
   printf("Saisir nom:");
=======
int ask(char *name, char* fonction, int nb_secu, void *sock_fd)
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

   strcpy(name,msg);

   printf("Name:%s\n",name);  

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

   strcpy(fonction,msg);

   printf("Fonction:%s\n",fonction);

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

   nb_secu = atoi(msg);

   printf("Nb_secu:%i\n",nb_secu);

   bzero(client_message,2000);
   bzero(msg,100);

   read_size++;

   /*printf("Saisir nom:");
>>>>>>> dde80e0c9f0efb3f65c152835b84b5fb18ac6eae
   scanf("%s",name);
   printf("\nSaisir fonction:");
   scanf("%s",fonction);
   printf("\nSaisir numero de secu:");
   scanf("%d",&nb_secu);
<<<<<<< HEAD
=======
   return nb_secu;*/
>>>>>>> dde80e0c9f0efb3f65c152835b84b5fb18ac6eae
   return nb_secu;
   
}



<<<<<<< HEAD
int creat_account(FILE*bdd)
=======
int creat_account(FILE*bdd, void* sock_fd)
>>>>>>> dde80e0c9f0efb3f65c152835b84b5fb18ac6eae
{
   int cursor;
   int trouve = 0;
   int verif=0;

   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *name = malloc(20*sizeof(char));
   char *fonction = malloc(20*sizeof(char));

   int nb_secu = 0;
   int cmp=1;
   int stck = 0;

   FILE *tmp = NULL;
   tmp = fopen("tmp.txt","a");

   int nb_lignes = nb_lines(bdd);

   printf("\n______CREATION DE COMPTE______\n");

<<<<<<< HEAD
   nb_secu = ask(name,fonction,nb_secu);
=======
   nb_secu = ask(name,fonction,nb_secu, sock_fd);
>>>>>>> dde80e0c9f0efb3f65c152835b84b5fb18ac6eae

   bdd = fopen("bdd.txt","r");
   while((cursor = fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%d %s %s",&verif,dust1,dust2);
      printf("%d %s %s\n",verif,dust1,dust2);
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
	 fprintf(tmp," %d %s %s\n",nb_secu,name,fonction);
      }
      else
      {
	 while((cursor = fgetc(bdd)) != EOF )
	 { 
	    fscanf(bdd,"%d %s %s",&verif,dust1,dust2);

	    if(verif < nb_secu)
	    {
	       fprintf(tmp," %d %s %s\n",verif,dust1,dust2);
	       if(cmp == nb_lignes)
	       {
		  fprintf(tmp," %d %s %s\n",nb_secu,name,fonction);
		  break;
	       }
	    }
	    if(verif > nb_secu)
	    {
	       if(stck == 0)
	       {
		  fprintf(tmp," %d %s %s\n",nb_secu,name,fonction);
		  fprintf(tmp," %d %s %s\n",verif,dust1,dust2);
	       }
	       if(stck != 0 && cmp < nb_lignes +1)
	       {
		  fprintf(tmp," %d %s %s\n",verif,dust1,dust2);
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
   free(name);
   free(fonction);

   return 0; 
}


void delete_account(int nb_secu,FILE* bdd)
{
   int cursor = 0;
   int verif = 0;
   int cmp = 0;
   int check = 0;
   int nb_lignes = nb_lines(bdd);

   char *dust1 = malloc(10*sizeof(char));
   char *dust2 = malloc(10*sizeof(char));


   bdd = fopen("bdd.txt","r");
   FILE * tmp = NULL;
   tmp = fopen("tmp.txt","a");

   fseek(tmp,0,SEEK_SET);
   fseek(bdd,0,SEEK_SET);

   while((cursor = fgetc(bdd)) != EOF)
   {  
      fscanf(bdd,"%d %s %s",&verif,dust1,dust2);

      if(verif == nb_secu)
      {
	    printf("\ncompte supprime\n");
	    check = 1;
      }
      else
      {
	 if(cmp <nb_lignes )
	 {
	    fprintf(tmp," %d %s %s\n",verif,dust1,dust2);
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
   fclose(tmp);
   fclose(bdd);
}



/*
int main(int argc,char **argv)
{
   FILE* bdd = NULL;
   bdd = fopen("bdd.txt","a");
   fclose(bdd);
   int  test = creat_account(bdd);
   test = test+1;
   delete_account(5,bdd);
   return EXIT_SUCCESS;
}

*/




