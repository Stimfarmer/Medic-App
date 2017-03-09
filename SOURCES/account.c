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
* \brief Informations relatives d'un user
*/



typedef struct{
   char *name ;
   char *surname ;
   char *fonction;
   int nb_secu;
   char *mdp;
   char *login;
   char *year;
} user;



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

/**
* \brief Fonction calculant le nombre de ligne d'un fichier passé en paramètre
* \param bdd fichier dont on veut connaître le nbre de ligne
* \return retourne le nbre de ligne du fichier
*/
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
/**
* \brief Fonction demandant la saisie d'info pour un user
* \param usr user que l'on veut créer
* \return retourne le numero de secu du user
*/
int ask(user usr)
{
   printf("Saisir nom:");
   scanf("%s",usr.name);
   printf("\nSaisir prénom:");
   scanf("%s",usr.surname);
   printf("\nSaisir fonction:");
   scanf("%s",usr.fonction);
   printf("\nSaisir numero de secu:");
   scanf("%d",&usr.nb_secu);
   printf("\nSaisir mdp:");
   scanf("%s",usr.mdp);
   

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

   return usr.nb_secu;
   
}


/**
* \brief Fonction permettant à un user de se connecter
* \details L'utilisateur rentre ses login / mdp, s'ils sont présents dans la bdd, il est connecté
* \param usr user qui souhaite se connecter
* \param bdd base de donnée contenant les mdp / login
* \return retourne 0 si connecté, -1 sinon
*/

int inlog(user usr,FILE* bdd)
{
   char *log = malloc(20*sizeof(char));
   char *mdp = malloc(20*sizeof(char));
 
   usr = alloc_user(usr);
 
   int cursor;
   bdd = fopen("bdd.txt","r");

   printf("\nLogin:");
   scanf("%s",usr.login);
   printf("\nMdp:");
   scanf("%s",usr.mdp);
   

   while((cursor =fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%d %s %s %s %s %s %s",&usr.nb_secu,usr.name,usr.surname,usr.fonction,log,mdp,usr.year);
      if(strcmp(log,usr.login)==0 && strcmp(mdp,usr.mdp) == 0)
      {
	 printf("\nLOG IN SUCCESS\n");
	 return 0;
      }
   }

   printf("\nERREUR MDP OR LOG\n");

   fclose(bdd);
   desalloc_user(usr);
   free(log);
   free(mdp);
   return -1;

}


/**
* \brief Fonction permettant la création d'un compte
* \details la fonction verifie si le nouveau compte n'existe pas déjà, sinon, elle place le compte au bon endroit dans la bdd (triée en fonction du nb_secu)
* \param bdd base de donnée recueillant les comptes
* \return retourne -1 si le compte existe déjà, 0 sinon
*/


int creat_account(FILE*bdd)
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

   printf("\n______CREATION DE COMPTE______\n");

   nb_secu = ask(usr);

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

/**
* \brief Fonction permettant la suppréssion d'un compte de la bdd
* \details verifie si les infos sont bien contenues dans la bdd
* \param nb_secu numero de secu du compte que l'on veut supprimer
* \param bdd base de donnée contenant les comptes
* \return ne retourne rien
*/


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


/*

int main(int argc,char **argv)
{
   user usr;
   FILE* bdd = NULL;
   bdd = fopen("bdd.txt","a");
   fclose(bdd);
   int  test = creat_account(bdd);
   test = test+1;
 //delete_account(7,bdd);
   int result = inlog(usr,bdd);
   result = result +1;
   return EXIT_SUCCESS;
}
*/





