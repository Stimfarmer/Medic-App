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

char * client_cert ="-----BEGIN CERTIFICATE-----\n"
"MIIDCTCCAfGgAwIBAgIJAOhCj5hm8aW+MA0GCSqGSIb3DQEBCwUAMBsxDDAKBgNV\n"
"BAMMA01vaTELMAkGA1UEBhMCTkwwHhcNMTcwMzA5MTcyMTEyWhcNMTcwNDA4MTcy\n"
"MTEyWjAbMQwwCgYDVQQDDANNb2kxCzAJBgNVBAYTAk5MMIIBIjANBgkqhkiG9w0B\n"
"AQEFAAOCAQ8AMIIBCgKCAQEAyVyGBRDx4NplLXxQSAK9PkilovHV90OlAd9MkDyw\n"
"iCHvZ7cT62v6s07Lchh/FGaXJuXI4PGSWALP8d/q+PPrA+XP4G5sWIyvwx9UB3HX\n"
"5hPRHe/kh87Ss0B1ZEnC0KQWw951qWzCo2FmSWutwGRYc45AGeUMPO9doeHkmz3N\n"
"Bhs4E9bwEPk58+DLxAWBIYLLyb/2Tlm67hEFKQC/8ljsNmgK862+WIFWXvghSojU\n"
"+yBHw5T3/C1JNBab7F/eL/Iy8zK1juD/u6fRo5HrqCfU01fHiudJFIEDPwhLuN8+\n"
"1GymMRbY7SCAWcGKWo8WgxglRXVrfJJyk8h/WRMY+00PPwIDAQABo1AwTjAdBgNV\n"
"HQ4EFgQUZBMReWkld21HSL3PSOeBa4PtD4IwHwYDVR0jBBgwFoAUZBMReWkld21H\n"
"SL3PSOeBa4PtD4IwDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAr5yk\n"
"ydShOI7hOFHc1P4qxSuZJs8dzjCzWLSoHQxLYLXtXCCiA9/LM+uYoHuTb+2GietE\n"
"Y+Fgj6h2E/3SqVK66WDEthEmATrHt2Vm9FjqF6CjXH3NzNGfyqTbrzjEq4oC3zHw\n"
"O0J6km3MUPHDRLNPKBmd7HE96V7BCttr41gczQjUAd2bknMiOU1PlSbn4turnl1Y\n"
"GItrjSOz1IEJanx/nLbkL43iOls/Nv1NvFJSUHIJIE6ssbHSe7kvc0fR2fQcbK4m\n"
"9S0lNMkp0fif5jP0+TujdoO1kEbIYvcFDu4c8KrBLTY3gmq+m5V7dAGWtiNobDsg\n"
"CiIK2zXt/tJG1uxPzA==\n"
"-----END CERTIFICATE-----\n";

char * client_key ="-----BEGIN PRIVATE KEY-----\n"
"MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQDJXIYFEPHg2mUt\n"
"fFBIAr0+SKWi8dX3Q6UB30yQPLCIIe9ntxPra/qzTstyGH8UZpcm5cjg8ZJYAs/x\n"
"3+r48+sD5c/gbmxYjK/DH1QHcdfmE9Ed7+SHztKzQHVkScLQpBbD3nWpbMKjYWZJ\n"
"a63AZFhzjkAZ5Qw8712h4eSbPc0GGzgT1vAQ+Tnz4MvEBYEhgsvJv/ZOWbruEQUp\n"
"AL/yWOw2aArzrb5YgVZe+CFKiNT7IEfDlPf8LUk0FpvsX94v8jLzMrWO4P+7p9Gj\n"
"keuoJ9TTV8eK50kUgQM/CEu43z7UbKYxFtjtIIBZwYpajxaDGCVFdWt8knKTyH9Z\n"
"Exj7TQ8/AgMBAAECggEBAKqqCsUceYV0VTDaHkkKWU8Cz7BtfEYYRTBzDDdr2Mto\n"
"x7qGBxIP8OosrG5FqN+nnBDRzRIqbTi/GXQNrt/WDKi/DRNRQ+9BPm8IjHXK3YwZ\n"
"27Gd83nPp/FZfqXDhqTDjq/wSpWQot9K3CD6bMeVxQob1OSdp2O2FQvxo4qYNL8o\n"
"rb+GG/NvUFFZ9u92/fdU0F7f5bM4tyn08iZjlURSpMgniilt7OU0dNaMdFO+lgj+\n"
"l2YnQKWx3JJjuGLbSmItR0dqybH41gQ1rJbtgQRtLiPJP51zBAgUDjRpqxmtzIOt\n"
"zQVxYAkVp6xelwAxFLXTirkbSLCJYNVSR31B0RJxObECgYEA6NY2lTJhNdm2iBnn\n"
"5HftrOEsuxpjFNM8d29aU4zZrKqDjDExdVPrkESseCA6zzmInxRrCtn6Z4n2kC+L\n"
"xqJzuRZgksFxQaa3qyD6U2Lm+W37pvvZ4D+R8OhfT5OFm9cKCwX5ZvvU8tl428se\n"
"HsBeEgZzEKCQfguJsg7iGKOCSnsCgYEA3WS1qMKc1CiSJSqJEGDl83xjGClqqNuW\n"
"gnIo5AXRpfpRWBsctz7tF4H/JD8rT2EGrtQ0zKMhynZ1kn79O8x4W6md6cdslxPO\n"
"RamsntC5Rd9+5g+PoPPeIN9bg20a2tUTQgdUnV+q/PXFIaNloENdxucxd4lAeMYQ\n"
"AnyT81qqpQ0CgYEA3APPgBwjD6eye9k+7v0EciA1DHhauAsDUDjAucKWzjoyI/84\n"
"JQ4vwhuifcC20aokF25L1+0yWTRc8NJ82i4X/AXRK9vvEYJgfCylXrZOW9Q5wAis\n"
"UOLBHUv3BFL6JdCFhPUYPuAo0XH+anL8+0H7ojzF7ihLYU+1xbOaon2fWJECgYEA\n"
"uFTvw3E17pdRB0zhDhf/nbkFMMrbGXirWjT+wsvtgEpoi/kW8aOc+COucVl/D/py\n"
"/AGHbJKuwcnbnnvJEI7v7zHMXydBzJAbAHmDVj/m4rxxBdATACnL2obdxRGEeItM\n"
"G3/K/bT3wuJGnflT1hoA7Gj2ENbgLS6LieZ7sPTee8kCgYAFCAlDYmJ3diFxTfdP\n"
"Djcmg5lRoazndoTh1SbNjNiGmZ5u18A2diSQkSbFno9kL6vy5DW69jgHdZtDXdav\n"
"tZvxvNDOoX3utE8AgR8ws3I522lYX3Ihx6D3tB9NrrOOWUWBcTppmyOOzkjBTJWY\n"
"jmraDrCnTCx+2I8p7PKDy3iDNA==\n"
"-----END PRIVATE KEY-----\n";
 
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
    LoadCertificates(ctx, client_cert/*"client.crt"*/, client_key/*"client.key"*/);
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
