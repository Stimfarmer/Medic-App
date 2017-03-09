#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct{
   char *name;
   char *fonction;
   int nb_secu;
   char *droit;
   char *mdp;
} user;

void init_user(user usr);

int nb_lines(FILE* bdd);

int ask(char *name, char* fonction, int nb_secu, void *sock_fd);

int creat_account(FILE*bdd, void* sock_fd);

void delete_account(int nb_secu,FILE* bdd);

#endif
