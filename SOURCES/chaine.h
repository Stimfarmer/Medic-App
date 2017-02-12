#ifndef _CHAINE_H_
#define _CHAINE_H_

/* chaine.h est un header pour définir les fonctions isalpha, isalnum, isnum 
et aussi enlever les caractères d'échappement */

int is_alpha( char*); // renvoit 1 si la chaine est full alpha, 0 sinon

int is_alnum( char*); // renvoit 1 si la chaine est alphanumérique, 0 sinon

int is_num( char*); // renvoit 1 si la chaine est full numérique, 0 sinon

char* delete_end_char( char*); //supprime les caractères de fin de chaine

#endif
