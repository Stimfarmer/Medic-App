#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <pthread.h>
#include "fonction_serveur.h"
#include "chaine.h"
 
#define FAIL    -1

char* serveur_cert ="-----BEGIN CERTIFICATE-----\n"
"MIIC+zCCAeOgAwIBAgIJAJxlWJFYLVJpMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\n"
"BAMMCWxvY2FsaG9zdDAeFw0xNzAzMDkxNzA5MTNaFw0xNzA0MDgxNzA5MTNaMBQx\n"
"EjAQBgNVBAMMCWxvY2FsaG9zdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"
"ggEBAM0N6m6lTMha/dQi/G+l31voO6lZauOKwh7lYgmryMw72yM9GEaf1+HagLQs\n"
"cljkOXnvJ/AVTTmbc+taLJQlEnFIBFk9LaLtuKlJ+aGONX7cLw0uFiPS8lXH3XBB\n"
"DKmMR8YcYZNe33UboRoHaDwVxEn96RyLG3ilVVgNl4pwdkjYrqyOqoih1/OpuMuw\n"
"zgax+pGYM9JUnGkysyatpC6meCPLxJfJogY7tEw+1QoxShGKgGoHFCTw+3Vbb8Fu\n"
"3KL8xqPu8tEOFGyIUhWe0gvJq5g7doCPmpHeX+bPlxld189FnbI98C55bVbAsEta\n"
"5cF8aFyTn3fsxTZw/OirBqgM0jECAwEAAaNQME4wHQYDVR0OBBYEFGdC64pqRTi4\n"
"Zxb2TwhAXyAQDlN6MB8GA1UdIwQYMBaAFGdC64pqRTi4Zxb2TwhAXyAQDlN6MAwG\n"
"A1UdEwQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBALd7nR606bb4lBJVKYtt5SZi\n"
"oFkBj19FxkAXzndmMfsfp4ZgSrsDoaLOyzT8QJTOtY7vEG/Rky69KrTVn/VcBYsV\n"
"p3JqdgeMvRKr63WoSvwg1NWksCx57884XhwQCNAXq88fXKbvrbwnQxgrCQvp6xfZ\n"
"0xAR+Px5syhug379c+5/a5LK8ocO/GxrGDD4zdAeI/ahE/NaErRKiKL0UU2kUFxb\n"
"Acsyg8GY3HYG2mm/mGX8byZ/iPieyZVFPaGubxdGitGqxubesfvP3ypR/iwd/gxR\n"
"vJL09PK8bOy8O/HbznEGw0JeBmkaqMBzCu06MW3tHF1g0O0/pcO01Zfv3LHTNKQ=\n"
"-----END CERTIFICATE-----\n";

char* serveur_pem ="-----BEGIN PRIVATE KEY-----\n"
"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDNDepupUzIWv3U\n"
"Ivxvpd9b6DupWWrjisIe5WIJq8jMO9sjPRhGn9fh2oC0LHJY5Dl57yfwFU05m3Pr\n"
"WiyUJRJxSARZPS2i7bipSfmhjjV+3C8NLhYj0vJVx91wQQypjEfGHGGTXt91G6Ea\n"
"B2g8FcRJ/ekcixt4pVVYDZeKcHZI2K6sjqqIodfzqbjLsM4GsfqRmDPSVJxpMrMm\n"
"raQupngjy8SXyaIGO7RMPtUKMUoRioBqBxQk8Pt1W2/Bbtyi/Maj7vLRDhRsiFIV\n"
"ntILyauYO3aAj5qR3l/mz5cZXdfPRZ2yPfAueW1WwLBLWuXBfGhck5937MU2cPzo\n"
"qwaoDNIxAgMBAAECggEBAIFmL7OFMtAYw6d0MJMQPMSV59h1qgNPlDLFFDoAIuXn\n"
"O+Gf80rWIfjP13pbtgksdKzi4/QYrTQWSm+R5yHRxzKffMcH2V3A4FBMc3Ck+lNF\n"
"BZgupiATRYQmbTIiiqgP81csn9LNAvy249I2LGwGXGlJBytRCZP32cSIR0JAuBQ9\n"
"LgP4eS1OZQo4CP/vDWmhsc8gPuAUVeYK7+4IWMQ0k5XQASZ/yDpSP+SiK/SCrVp0\n"
"Gos4CI1n7+UUw9NouABhgMvVhdqiNZiyVQ5QAJyKbmmTFijBkKnjmc9/CfqnZZR3\n"
"JCe0KjiSXTGxktPoActii6+2gExQIj8+n7taW84MU+UCgYEA/+8pFYfxfSshu1ox\n"
"YhQVbPtHAm7w/iA5UJrEsgMYyjsFcrfpLloWYIQh3GOLK8sUnz/zguyGgQF++nD/\n"
"djHdD5JEUp49hHSUpbNXI55+36ktsVnNrUHOB8E9I2u78DEgRxS+t8JxIm8DawW5\n"
"sXgrblnjvfkJvSrxTRkO5QMN4NsCgYEAzRtoVe8haB51267qJcefDRV0DH//fsA2\n"
"le+l1DnFDnCiq/xIef98LYpi0/ongj401Cz9BLxKevdbhvkj7dj9MT7G++7ydRks\n"
"jgruuBYOYelQBGbefcQcxpT/0FzzGPvgaj0OAXTWT0y+OhCrRvi5/urlzt7/EZTU\n"
"vND6MZmOUOMCgYBwxyOUgE4SuQHpPApMzMrQNj4JPXfmk66N7VVFmAXBf5acAZy4\n"
"NyxrJW6CXj8Ccj/JGPYmODHhARbCJy77F2fxJ6QBwAcdqf2iss9wO7RuvG8NGqR+\n"
"d2PhmO5nAF9VrWAq0mJkOUN/R+n6rBAyuuBwhR3NJcN730NrlOIyWlEwRwKBgQCQ\n"
"p6zNNqr78Me0mUdgP8qIAsJm8nsyXMPiuTLeW6QEDCJeY76F9E8TG0+aKwpkTBUc\n"
"M2J7TYTF9uChb+MMI4Zmjdzu39Y2CfwnbcRxJnUcTt11TUvLrV2SHJHS1UtKKpJa\n"
"uxa+pjVjlj9vWkRB+LfM9sP/Q7znHG1xgX2DL8Z0ewKBgC4coFlJA+0yeyiiL3N1\n"
"pMsdnZMFqWlvAWkloeWRX1/cEEWWzoPzQNhpSIsUZX3shkNpLKivBkiqeaJGm0ly\n"
"0aGKlKYk/OXufayAkqqhAavXPoOtWU6PPH8oQMp5Xr9npExYgzqgRxM3hMFYo5w6\n"
"0hbKkpL9+K3vcVIEe6FEhB1i\n"
"-----END PRIVATE KEY-----\n";
 
int OpenListener(int port)
{   int sd;
    struct sockaddr_in addr;
 
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if( setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)))
    {
       perror("Erreur du set de la socket");
       return 1;
    }
    
    if ( bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("Bind Impossible");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Impossible de configurer le port d'écoute \n");
        abort();
    }
    return sd;
}
 
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
 
}
SSL_CTX* InitServerCTX(void)
{   SSL_METHOD *method;
    SSL_CTX *ctx;
 
    OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
    SSL_load_error_strings();   /* load all error messages */
    method = TLSv1_2_server_method();  /* create new server-method instance */
    ctx = SSL_CTX_new(method);   /* create new context from method */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /*if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
        ERR_print_errors_fp(stderr);

    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
        ERR_print_errors_fp(stderr);*/
    
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
    
    /*SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    SSL_CTX_set_verify_depth(ctx, 4);*/
}
 
void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
    if ( cert != NULL )
    {
        printf("Certificats serveur:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("Client simple sans certificat\n");
}
 
void Servlet(SSL* ssl) /* Serve the connection -- threadable */ // succède à "connection_handler"
{   char buf[1024];
    char cmd[200];
    int *log;
    char *liste_cmd;
    //char reply[1024];
    int sd, bytes;
    //const char* HTMLecho="modifié \n";
 
    log = (int*)malloc(1*sizeof(int));
    *(log) = 0;

    if ( SSL_accept(ssl) == FAIL )     /* do SSL-protocol accept */
        ERR_print_errors_fp(stderr);
    else
    {
        ShowCerts(ssl);        /* get any certificates */
        //bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */ // chercher le segfault ici
	liste_cmd = "Bonjour!\n1- auth pour authentifier\n2- deauth pour de loger\n3- insc pour inscrire\n4- help pour afficher l'aide\n5- quit pour quitter le serveur\nBonne navigation\n";
	SSL_write(ssl,liste_cmd,strlen(liste_cmd));
        do
	{
	  bytes = SSL_read(ssl, buf, sizeof(buf));
	  if ( bytes > 0 )
	  {
	      if((delete_end_char(cmd,sizeof(cmd),buf))==-1)
	      {
		perror("Erreur supression caractère de fin!");
		break;
	      }
              printf("Client msg: !%s!\n", buf);
	      //sprintf(reply, "reçu \n", buf);   /* construct reply */
	      //SSL_write(ssl, reply, strlen(reply)); /* send reply */
              printf("La commande serveur est %s\n",cmd);
	      function_to_select(ssl, cmd, log);
              bzero(buf,1024);
	      bzero(cmd,200);

	      //bytes = SSL_read(ssl, buf, sizeof(buf));
	  }
	  else
	  {

	      ERR_print_errors_fp(stderr);
	      break;
	  }
	  
	      
	}
	while(bytes > 0);
    }
    sd = SSL_get_fd(ssl);       /* get socket connection */
    //SSL_free(ssl);         /* release SSL state */
    close(sd);          /* close connection */
}
 
int main(int argc, char **argv)
{   SSL_CTX *ctx;
    int server;//*new_sock;
    char *portnum;
 
    if(!isRoot())
    {
        printf("Ce programme doit être executé en contexte root/sudo \n");
        exit(0);
    }
    /*if ( count != 2 )
    {
        printf("Syntaxe: %s <portnum>\n", strings[0]);
        exit(0);
    }*/
    SSL_library_init();
 
    portnum = argv[1]; /******************* 8888 ******************/
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, serveur_cert/*"server.cert"*/, serveur_pem/*"server.pem"*/); /* load certs */
    server = OpenListener(atoi(portnum));    /* create server socket */
    printf("Socket SSL créée!\n");
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    SSL *ssl;
    int client;
 
        while ( (client = accept(server, (struct sockaddr*)&addr, &len)) )
	{/* accept connection as usual */
	  printf("Connection: %s: %d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	  ssl = SSL_new(ctx);              /* get new SSL state with context */
	  SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
	  //ShowCerts(ssl);
	  pthread_t sniffer_thread;
	  /*new_ssl = malloc(1);
	  *new_sock = client;*/
	  
	  if( (pthread_create( &sniffer_thread , NULL ,  (void*)(Servlet) , (void*) ssl)) < 0)
	  {
	      perror("Impossible de créer un thread");
	      return 1;
	  }
	  //Servlet(ssl);/* service connection */
	}
    
    close(server);          /* close server socket */
    SSL_CTX_free(ctx);
    return 0; /* release context */
 }
