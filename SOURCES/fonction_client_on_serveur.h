#ifndef _FONCTION_CLIENT_ON_SERVEUR_H_
#define _FONCTION_CLIENT_ON_SERVEUR_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Ce header sert à définir les fonctions disponibles pour le client
potentiel et développées sur le serveur */

int authentification_function( void *); // fonction d'authentification d'un client
int inscription_function( void *); // fonction de d'inscription au serveur de l'hopital
int affichage_aide_function( void *); // fonction d'affichage d'aide sur le serveur pour un client
int quit_function( void *); // fonction pour quitter le serveur

#endif
