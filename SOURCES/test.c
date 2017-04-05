#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
   char *name ;
   char *surname ;
   char *fonction;
   int nb_secu;
   char *mdp;
   char *login;
   char *year;
} user;

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

int change_mdp(user usr,FILE* bdd)
{
   int verif;
   int cmp=0;

   char *old_passwd = malloc(20*sizeof(char));
   char *new_passwd = malloc(20*sizeof(char));
   char *dust1 = malloc(20*sizeof(char));
   char *dust2 = malloc(20*sizeof(char));
   char *dust3 = malloc(20*sizeof(char));
   char *dust4 = malloc(20*sizeof(char));
   char *dust5 = malloc(20*sizeof(char));
   char *dust6 = malloc(20*sizeof(char));
   
   int nb_ligne = nb_lines(bdd);
   bdd = fopen("bdd.txt","r");
   FILE* tmp = NULL;
   tmp = fopen("tmp.txt","a");
   int cursor;

      

   while((cursor = fgetc(bdd)) != EOF)
   {
      if(cmp < nb_ligne)
      {
         fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);
         if(strcmp(dust4,usr.login)==0)
         {
            printf("Enter old password\n");
            scanf("%s",old_passwd);
            if(strcmp(old_passwd,dust5)==0)
            {
               printf("New password\n");
               scanf("%s",new_passwd);
               bzero(usr.mdp,20);
               sprintf(usr.mdp,new_passwd);
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

   while((cursor = fgetc(bdd)) != EOF)
   {
      if(cmp < nb_ligne)
      {
         fscanf(bdd,"%d %s %s %s %s %s %s",&verif,dust1,dust2,dust3,dust4,dust5,dust6);
         if(strcmp(dust4,usr.login)==0)
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
   int ret1 = remove("bdd.txt");
   if(ret1 != 0)
   {
      printf("ERREUR: Suppression fonction change_log\n");
   }
   int ret2 = rename("tmp.txt","bdd.txt");
   if(ret2 != 0)
   {
      printf("ERREUR: Rename fonction change_log\n");
   }
      

   free(dust1);
   free(dust2);
   free(dust3);
   free(dust4);
   free(dust5);
   free(dust6);
   free(new_passwd);
   free(old_passwd);
   fclose(bdd);
   fclose(tmp);   
   return 0;

}

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

 

}



int main(int argc,char **argv)
{
   FILE* bdd = NULL;
   user usr = alloc_user(usr);
   usr.login = "esapin";
   
   generate_passwd(usr);
   printf("%s\n",usr.mdp);
 //  result = change_mdp(usr,bdd);
 //  printf("result:%d\n",result);

   return 0;
}