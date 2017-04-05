#ifndef _CRYPTAGE_H_
#define _CRYPTAGE_H_

/* Ce header sert à définir les fonctions de cryptage et décryptage des fichiers présents localement sur la machine d'un utilisateur */

int crypt_simple(char *file_name); // fonction de cryptage d'un fichier, renvoie -1 si cryptage impossible

int uncrypt_simple(char *file_name); // fonction de décryptage d'un fichier, renvoie -1 si décryptage impossible

#endif
