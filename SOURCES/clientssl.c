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
#include "cryptage.h"
 
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
    int server,compteur_dl=0,n=0;
    FILE *dl_cp;
    char *log[1024];
    //int log_or_not; // 0 = non log 1 = log;
    SSL *ssl;
    char buf[2000];
    int bytes;
    char *hostname, *portnum;
    char message[1000] , server_reply[2000];
    char *argfile,*current_dir_serveur,*password;
    char *cmd_vim;
    char user[20],file[20],file_to_read[20];
    char vim_script_dir[500];
    
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
    LoadCertificates(ctx, "SOURCES/client.crt", "SOURCES/client.key");
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
	    fflush(stdout);
	    fflush(stdin);
	    fgets(message,sizeof(message),stdin);

	    if((strcmp(message,"clear\n")) == 0){
	    	system("clear");
	 	continue;
	    }	    

	    if((strcmp(message,"readfile\n")) == 0)
	    {
	       fprintf(stdout,"Nom du fichier à ouvrir:");
	       //scanf("%s *[^\n]", file_to_read);
	       //scanf("%s",file_to_read);
	       fgets(file_to_read,sizeof(file_to_read),stdin);
	       delete_end_char(file_to_read,sizeof(file_to_read),file_to_read);
	       uncrypt_simple(file_to_read);
	       if((dl_cp = fopen(file_to_read,"r")) == NULL){
	          perror("Erreur ouverture fichier téléchargé");
	          exit(2);
	       }
	       while(fgets(buf,sizeof(buf),dl_cp) != NULL)
	       {
	          fprintf(stdout,"%s",buf);
	       }
	       fclose(dl_cp);
	       crypt_simple(file_to_read);
	       bzero(message,1000);
	       n = strlen(message);
               if (n>0 && message[n-1]=='\n') { message[n-1] = 0; }
	       bzero(file_to_read,20);
	       continue;
	    }

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
	          bzero(user,20);
	          delete_end_char(log[2],1024*sizeof(char),log[2]);
		  strcpy(user,log[2]);
               }

	       else if( strcmp(log[0],"Dec") == 0){
	          bzero(user,20);
	          //delete_end_char(log[2],1024*sizeof(char),log[2]);
		  strcpy(user,"You");
               }
	       else if( strcmp(log[0],"dl") == 0){
		  printf("Dl d'un fichier...\n");
		  puts("\nServer>");
	    	  puts(server_reply);
	          bzero(server_reply,2000);
		  //printf("Log[2]: %s\n",log[2]);
		  sprintf(file,"%i",compteur_dl);
		  strcat(file,"_dl");
		  dl_cp = fopen(file,"w");

		  //n = atoi(log[2]);
		  while(SSL_read(ssl,server_reply,2000))
		  {
		     if ((strcmp(server_reply,"end_dl")) == 0)
		     {
		        break;
		     }
		     fprintf(dl_cp,"%s",server_reply);
		     bzero(server_reply,2000);
		  }
		  fclose(dl_cp);
		  crypt_simple(file);
		  printf("New file downloaded: %s\n",file);
		  compteur_dl++;
		  bzero(server_reply,2000);
	    	  bzero(message,1000);
		  bzero(file,20);
	 	  continue;
	       }
            }
	    
  	    delete_end_char(message,sizeof(message),message);

	    if(strcmp(server_reply,"quit") == 0)
	    {
	       sleep(1);
               printf("Deconnexion...\n");
	       break;
	    }
	    
	    if( strcmp(message,"") != 0)
	    {
	    	cmd_vim = strtok(message," "); // on fractionne la chaine pour récupérer l'argument s'il existe
	    	if(strcmp(cmd_vim,"vim")==0)
	    	{
		  if( getcwd( vim_script_dir, sizeof(vim_script_dir) ) == NULL)
		  {
		    printf("\n Erreur getcwd() \n");
		  }
		  printf("\n Chargement de vim distant... \n");
		  char vim_buf[300];
		  argfile=strtok(NULL," ");
		  if ( argfile == NULL ) // si il n'y a pas d'arguments pour vim 
		  {
		    //printf("Server_reply: %s\n",server_reply);
		    strsplit(server_reply,log," ");
		    current_dir_serveur = log[1];
		    password = log[0];
		    //printf("Current_dir_serveur: %s\n",current_dir_serveur);
		    sprintf(vim_buf,"%s/vim_script.sh %s %s %s",vim_script_dir, hostname, password, current_dir_serveur); // mettre le bon path
		    //sprintf(vim_buf,"sshpass -p %s ssh -o StrictHostKeyChecking=no romain@%s", password, hostname);
		    system(vim_buf);
		  }
		  else
		  {
		    //printf("Server_reply: %s\n",server_reply);
		    strsplit(server_reply,log," ");
		    current_dir_serveur = log[1];
		    password = log[0];
		    //printf("Current_dir_serveur: %s\n",current_dir_serveur);
		    sprintf(vim_buf,"%s/vim_script.sh %s %s %s %s", vim_script_dir, hostname, password, argfile, current_dir_serveur); // idem 
		    system(vim_buf);
		  }
		//bzero(password,20);
	   	bzero(server_reply,2000);
		bzero(message,1000);
		continue;
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
