#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char**argv)
{
	FILE* ls = NULL;
	ls = fopen("ls.txt","r");
	char* ligne = malloc(80*sizeof(char));	
	char tmp;
	printf("Entrez valeur:\n");
	while(1)
	{
		if((tmp = getc(stdin))=='\t')
		{
			while(fgets(ligne,80,ls) != NULL)
			{
				printf("%s",ligne);
			}
		}
	}




	return 0;

}
