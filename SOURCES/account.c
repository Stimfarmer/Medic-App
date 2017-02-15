#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
   char name[10];
   char fonction[10];
   int nb_secu;
} user;


int nb_lines(FILE* bdd)
{
   int cursor;
   int nb_lignes = 0;
   while ((cursor = getc(bdd)) != EOF)
   {
      if(cursor == '\n')
      {
	 ++nb_lignes;
      }
   }
   return nb_lignes;
   fseek(bdd,0,SEEK_SET);
}


void creat_account(char *name, char *fonction,int nb_secu,FILE*bdd)
{
   int cursor;
   int trouve = 0;
   int verif=0;
   char dust1[10];
   char dust2[10];
   bdd = fopen("bdd.txt","r+");
   while((cursor = fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%s %s %d",&dust1,&dust2,&verif);
      printf("%s\n%s\n",dust1,dust2);
      if(verif == nb_secu)
      {
	 trouve = 1;
	 printf("Compte existant\n");
	 break;
      }
   }

   if(trouve == 0)
   {
      bdd = fopen("bdd.txt","a");
      fprintf(bdd," %s %s %d\n",name, fonction, nb_secu);
      fseek(bdd,0,SEEK_SET);
   }
   
}

void delete_account(int nb_secu,FILE* bdd)
{
   int cursor;
   int verif = 0;
   char choix[10];
   char dust1[10];
   char dust2[10];
   bdd = fopen("bdd.txt","r");
   while((cursor = fgetc(bdd)) != EOF)
   {
      fscanf(bdd,"%s %s %d",&dust1,&dust2,&verif);
      if(verif == nb_secu)
      {
	 printf("Voulez vous vraiment supprimer ce compte ?\n");
	 scanf("%s",choix);
	 if(!strcmp(choix,"oui"))
	 {
	    printf("\ncompte supprime\n");
	    break;
	 }
      }
   }
}
