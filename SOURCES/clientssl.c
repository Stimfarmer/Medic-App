#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "chaine.h"
#include "strsplit.h"
 
#define FAIL    -1
 
void gestionnaire (int numero)
{

}

int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
 
    if ( (host = gethostbyname(hostname)) == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}
 
SSL_CTX* InitCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        fprintf(stderr, "La clé privée ne correspond pas au certificat public \n");
        abort();
    }
}
 
void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}
 
int main(int argc, char **argv)
{   SSL_CTX *ctx;
    int server;
    char *log[20];
    //int log_or_not; // 0 = non log 1 = log;
    SSL *ssl;
    char buf[2000];
    int bytes;
    char *hostname, *portnum;
    char message[1000] , server_reply[2000];
    char *argfile;
    char *cmd_vim;
    char user[20];

    strcpy(user,"You");
    
 
    /*if ( count != 3 )
    {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }*/
    signal(SIGINT, gestionnaire);
    SSL_library_init();
    hostname=argv[1];
    portnum=argv[2];
 
    ctx = InitCTX();
    LoadCertificates(ctx, "client.crt", "client.key");
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
        ERR_print_errors_fp(stderr);
    else
    {   //char *msg = "Hello serveur";
 
        printf("Connecté et sécurisé par %s \n", SSL_get_cipher(ssl));
        ShowCerts(ssl);        /* get any certs */
        //SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
	buf[bytes] = 0;
	puts("\nServer>");
	puts(buf);
        //SSL_free(ssl);        /* release connection state */
	
	while(1)
	{
	    printf("%s>",user);
	    fgets(message,sizeof(message),stdin);
	    //Send some data
	    if( SSL_write(ssl , message , strlen(message) ) < 0)
	    {
		puts("Send failed");
		break;
	    }

	    //Receive a reply from the server
	    if( SSL_read(ssl , server_reply , 2000) < 0)
	    {
		puts("recv failed");
		break;
	    }

	    if( (strsplit(server_reply,log," ")) > 0){
	       delete_end_char(log[0],sizeof(log[0]),log[0]);
	       if( strcmp(log[0],"Log") == 0){
	          printf("Je suis log!!\n");
	          bzero(user,20);
	          delete_end_char(log[2],20*sizeof(char),log[2]);
		  strcpy(user,log[2]);
               }
            }
	    
  	    delete_end_char(message,sizeof(message),message);

	    if(strcmp(server_reply,"quit") == 0)
	    {
	       sleep(1);
               printf("Deconnexion...\n");
	       break;
	    }
	
	    if( strcmp(message,"") != 0){

	    	cmd_vim = strtok(message," "); // on fractionne la chaine pour récupérer l'argument s'il existe
	    	if(strcmp(cmd_vim,"vim")==0)
	    	{
	      	char vim_buf[300];
	      	argfile=strtok(NULL," ");
	     	 if ( argfile == NULL ) // si il n'y a pas d'arguments pour vim 
	      	{
			sprintf(vim_buf,"/home/esapin/Bureau/PROJET_RESEAU/SOURCES/vim_script.sh"); // mettre le bon path
			system(vim_buf);
	      	}
	      	else
	      	{
			sprintf(vim_buf,"/home/esapin/Bureau/PROJET_RESEAU/SOURCES/vim_script.sh %s", argfile); // idem 
			system(vim_buf);
	      	}
	   	 }
	    }

            puts("\nServer>");
	    puts(server_reply);
	    bzero(server_reply,2000);
	    bzero(message,1000);
	}
    }
    SSL_free(ssl);
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    printf("Déconnecté.\n");
    exit(2);
}
