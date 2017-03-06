#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <openssl/ssl.h>
 
int main(int argc , char *argv[])
{
   int sock;
   struct sockaddr_in server;
   char message[1000] , server_reply[2000];
     
   //Create socket    
   sock = socket(AF_INET , SOCK_STREAM , 0);
   SSL_CTX *ctx;

   init_openssl();
   ctx = create_context();

   configure_context(ctx);
   
   if (sock == -1)
   {
      printf("Could not create socket");
   }
   puts("Socket created");
     
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_family = AF_INET;
   server.sin_port = htons( 8888 );
 
   //Connect to remote server
   SSL *ssl;
   SSL_set_connect_state(ssl);
   
   if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
   {
      perror("connect failed. Error");
      return 1;
   }
     
   puts("Connected\n");

   if( SSL_read(ssl , server_reply , strlen(server_reply) < 0)
   {
       puts("recv failed");
   }

   puts("Server>");
   puts(server_reply);
   bzero(server_reply,2000);
     
   //keep communicating with server
   while(1)
   {
      printf("You>");
      scanf("%s" , message);
      
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

      if(strcmp(message,"quit") == 0)
      {
         printf("Deconnexion...\n");
         break;
      }
         
      puts("Server>");
      puts(server_reply);
      bzero(server_reply,2000);
      bzero(message,1000);
   }
     
   close(sock);
   printf("Deconnecte\n");
   return 0;
}
