#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct{
   char *name ;
   char *surname ;
   char *fonction;
   int nb_secu;
   char *mdp;
   char *login;
   char *year;
} user;

void init_user(user usr);

user alloc_user(user usr);

void desalloc_user(user usr);

int nb_lines(FILE* bdd);

void search_log(char *log,int number,char *stck);

int ask(user usr, void *ssl);

int creat_account(FILE*bdd, void* ssl);

int inlog(user usr,FILE* bdd, void* ssl, int*log);

void delete_account(int nb_secu,FILE* bdd);

#endif
