
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// open ssl related includes
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/engine.h>

// Socket related includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// Macro for Keys/Certificates
#define CA_CERT "cacert.pem"

// Macro for Engine
#define ENGINE_NAME "hed_engine"

// Default IP/PORT
#define DEFAULT_IP		"127.0.0.1"
#define	DEFAULT_PORT		5000
#define SECURE_COMM		TLS_client_method()
//#define SECURE_COMM		DTLS_client_method()


//typedef
// For Socket
typedef enum {
	SOCKET_IS_NONBLOCKING,
	SOCKET_IS_BLOCKING,
	SOCKET_HAS_TIMED_OUT,
	SOCKET_HAS_BEEN_CLOSED,
	SOCKET_OPERATION_OK
} timeout_state;

//extern
extern	int waitpid();

// Function Protoyping
void doClientConnect(void);


int main (int argc, char *argv[])
{

	//Print Heading
	port_printf("\n*****************************************\n");
	
	doClientConnect();

	return 0;
}

void doClientConnect(void)
{
	int 		err;
	int len, i,j;
	SSL_CTX		*ctx;
	SSL		*ssl;
	SSL_METHOD	*meth;
	uint8_t		buf[4096];	
	int		sock;
	struct sockaddr_in	server_addr;

	short int	s_port = DEFAULT_PORT;

	//int		sockstate;

	uint8_t		s_ipaddr[] = DEFAULT_IP;

	port_printf("s_ipaddr : %s\n", s_ipaddr);

	SSL_library_init();
	SSL_load_error_strings();

	meth = (SSL_METHOD*) SECURE_COMM;

	ctx = SSL_CTX_new(meth);

  //SSL_CTX_set_ciphersuites(ctx, "TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA");
	if (!ctx) {
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	if(!SSL_CTX_load_verify_locations(ctx, "ca.crt", NULL)){
		ERR_print_errors_fp(stderr);
		exit(1);
	}
 
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
	SSL_CTX_set_verify_depth(ctx, 1);

	/*********************************************************************/
	// Setting the Socket
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // IPPROTO_TCP
	if (sock == -1)
	{
		perror("socket");
		exit(1);
	}

	memset(&server_addr, '\0', sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(s_port);
	server_addr.sin_addr.s_addr=inet_addr((char *)s_ipaddr);

	// Connect to server
	 port_printf("\nConnecting to server ....\n");
	err = connect(sock, (struct sockaddr*)&server_addr,sizeof(server_addr));
	if ((err)==-1)
	{

		perror("Connection");
		err = close(sock);
		if (err == -1)
		{
			perror("close");
			exit(1);
		}		
		SSL_CTX_free(ctx);

		return;

	}

	port_printf("\nConnected to %s, port :0x%.4x\n", s_ipaddr, server_addr.sin_port);

	/**********************************************************************/
	// TCP Connection is ready
	// Estabish the SSL Connection

	// Set verify depth to 1
	SSL_CTX_set_verify_depth(ctx,1);

	ssl = SSL_new(ctx);
	if (ssl == NULL) 
	{
		port_printf("\nExit\n");
		exit(1);
	}

	// Assign the socket into the SSL structure
	SSL_set_fd(ssl, sock);
    port_printf("Connected with %s encryption\n", SSL_get_cipher(ssl));

	
	// SSL Perfrom Handshaking
	port_printf("Performing Handshaking .....\n");
	err = SSL_connect(ssl);
	if (err== -1)
	{
		ERR_print_errors_fp(stderr);
		port_printf("\nConnection Error!!!");
		exit(1);
	}

	port_printf("The cipher suite is: %s\n", SSL_get_cipher(ssl));
	port_printf("The SSL/TLS version is : %s\n", SSL_get_version(ssl));	

	/**********************************************************************/
			j =0;
			while(j < 101)
			{
				j++;
				err = SSL_write(ssl, &j, 1);
				if (err==-1)
				{
					ERR_print_errors_fp(stderr);
				} 
				
				len = SSL_read(ssl, buf, sizeof(buf) - 1);
				err = SSL_get_error(ssl, len);
  
				if ((err != 0) && (len == 0))
				{
					port_printf("\ndisconnected!!!");
				}
				else
				{
					for(i=0;i<len;i++)
					{
						printf("%c",(char)buf[i]);
					}
					port_printf("\n");
				}
				sleep(1);
			}

	port_printf("Connection Closed!!!\n");

	/**********************************************************************/
	// SSL Close
	err = SSL_shutdown(ssl);
	if (err==-1)
	{
		ERR_print_errors_fp(stderr);
		exit(1);
	}


	err = close(sock);

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	port_printf("it works!!!!");
}

