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

int ask(char *name, char* fonction, int nb_secu)
{
   printf("Saisir nom:");
   scanf("%s",name);
   printf("\nSaisir fonction:");
   scanf("%s",fonction);
   printf("\nSaisir numero de secu:");
   scanf("%d",&nb_secu);
   return nb_secu;
   
}



int creat_account(FILE*bdd)
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

   nb_secu = ask(name,fonction,nb_secu);

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




