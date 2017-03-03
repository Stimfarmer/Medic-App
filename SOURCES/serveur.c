#include "fonction_serveur.h"
#include "fonction_client_on_serveur.h"



/*int main(int argc , char *argv[])
{

   int socket_desc , client_sock , c , *new_sock;
   SSL_CTX *sslctx;
   SSL *cSSL;
   InitializeSSL();
   
   struct sockaddr_in server , client;
     
   //On crée la socket
   socket_desc = socket(AF_INET , SOCK_STREAM , 0);
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
     pthread_t sniffer_thread;
     new_sock = malloc(1);
     *new_sock = client_sock;
     
     sslctx = SSL_CTX_new( SSLv23_server_method() );
     SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE);
     int use_cert = SSL_CTX_use_certificate_file(sslctx, "/serverCertificate.pem" , SSL_FILETYPE_PEM );
     int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, "/serverCertificate.pem" , SSL_FILETYPE_PEM );
     cSSL = SSL_new(sslctx);
     SSL_set_fd(cSSL,new_sock);
     
     if(ssl_err <= 0)
     {
       ShutdownSSL();
     }
     
     SSL_write(cSSL, "Hi :3\n", 6);
      puts("Connection acceptée");
         
         
      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
       {
          perror("Impossible de créer un thread");
          return 1;
       }
         
       //Now join the thread , so that we dont terminate before the thread
       //pthread_join( sniffer_thread , NULL);
       puts("Handler assigned\n");
   }
     
   if (client_sock < 0)
   {
      perror("accept failed");
      return 1;
   }
     
   return 0;
}*/

int main(int argc, char **argv)
{
    int sock;
    SSL_CTX *ctx;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    sock = socket(AF_INET , SOCK_STREAM , 0);

    /* Handle connections */
    while(1) 
    {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        /*if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }*/

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
}
