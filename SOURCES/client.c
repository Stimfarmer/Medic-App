#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "chaine.h"
 
int main(int argc , char *argv[])
{
   int sock;
   struct sockaddr_in server;
   char message[1000] , server_reply[2000];
     
   //Create socket    
   sock = socket(AF_INET , SOCK_STREAM , 0);
   if (sock == -1)
   {
      printf("Could not create socket");
   }
   //puts("Socket created");
     
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_family = AF_INET;
   server.sin_port = htons( 8888 );
 
   //Connect to remote server
   if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
   {
      perror("connect failed. Error");
      return 1;
   }
     
   puts("Connected\n");

   if( recv(sock , server_reply , 2000 , 0) < 0)
   {
       puts("recv failed");
   }

   puts("SERVER>");
   puts(server_reply);
   bzero(server_reply,2000);
     
   //keep communicating with server
   while(1)
   {
      printf("YOU>");
      fgets(message,sizeof(message),stdin);
      //scanf("%s" , message);
      printf("Message à envoyer: %s\n",message);
      //Send some data
      if( send(sock , message , strlen(message) , 0) < 0)
      {
          puts("Send failed");
          return 1;
      }

      //Receive a reply from the server
      if( recv(sock , server_reply , 2000 , 0) < 0)
      {
          puts("recv failed");
          break;
      } 
      delete_end_char(message,sizeof(message),message);
      if(strcmp(message,"quit") == 0)
      {
         printf("Deconnexion...\n");
         break;
      }
         
      puts("SERVER>");
      puts(server_reply);
      bzero(server_reply,2000);
      bzero(message,1000);
   }
     
   close(sock);
   printf("Deconnecte\n");
   return 0;
}
