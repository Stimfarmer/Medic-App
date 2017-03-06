#include "fonction_serveur.h"
#include "fonction_client_on_serveur.h"



int main(int argc , char *argv[])
{

   int socket_desc , client_sock , c , *new_sock;
   
   struct sockaddr_in server , client; // client correspond à addr du serveur TLS simple
   //int sock;
   SSL_CTX *ctx;

   init_openssl();
   ctx = create_context();

   configure_context(ctx);
   
     
   //On crée la socket
   socket_desc = socket(AF_INET , SOCK_STREAM , 0); // correspond à sock du serveur TLS simple
   if (socket_desc == -1)
   {
      printf("Impossible de créer la socket");
   }
   puts("Socket créée");
     
   //On prépare la structure sockaddr_in
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons( 8888 );
   //On set la socket
   if( setsockopt( socket_desc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)))
   {
      perror("Erreur du set de la socket");
      return 1;
   }

   //On bind la socket
   if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
   {
      //On affiche le message d'erreur
      perror("Erreur de binding de la socket");
      return 1;
   }
   puts("Bind effectué");
     
   //On écoute désormais sur la socket
   listen(socket_desc , 3);

   //On accepte et on attend des connexions
   puts("En attente de connexion...");
   c = sizeof(struct sockaddr_in);
   
   while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
   {
     printf(" \n Nouvelle connexion détéctée \n");
     pthread_t sniffer_thread;
     new_sock = malloc(1);
     *new_sock = client_sock;
     
     //uint len = sizeof(client); // peut être changer client en client_sock 
     SSL *ssl;
     SSL_set_accept_state(ssl);
     const char reply[] = "test\n";
     
     printf("\n avant ssl write \n");
     SSL_write(ssl, reply, strlen(reply));
     printf("\n après ssl write \n");
      //puts("Connection acceptée");
         
         
      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
       {
          perror("Impossible de créer un thread");
          return 1;
       }
         
       //Now join the thread , so that we dont terminate before the thread
       //pthread_join( sniffer_thread , NULL);
       puts("Handler assigned\n");
       ssl = SSL_new(ctx);
       SSL_set_fd(ssl, client_sock);

       if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
       }
       else {
            SSL_write(ssl, reply, strlen(reply));
       }

       SSL_free(ssl);
       close(client_sock);
   }
     
   if (client_sock < 0)
   {
      perror("accept failed");
      return 1;
   }
   
   close(socket_desc);
   SSL_CTX_free(ctx);
   cleanup_openssl();
   
     
   return 0;
}

/*
int main(int argc, char **argv)
{
    int sock;
    SSL_CTX *ctx;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    sock = socket(AF_INET , SOCK_STREAM , 0);

    // Handle connections 
    while(1) 
    {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) 
	{
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
} */