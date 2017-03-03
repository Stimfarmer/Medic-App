#ifndef _FONCTION_SERVEUR_H_
#define _FONCTION_SERVEUR_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


/* fonction_serveur.h est un header servant à définir les fonctions
du serveur */

void *connection_handler(void *); // la fonction permettant de créer un thread pour chaque client

int function_to_select(void *, char*); // fonction permettant de choisir quelle fonction est à executer suite
//à la commande du client, renvoie -1 si aucune fonction ne correspond à la commande écrite

#endif 
