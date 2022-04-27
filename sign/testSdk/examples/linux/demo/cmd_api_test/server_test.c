#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//引擎名字
#define ENGINE_NAME "hed_engine"
//写入ECC私钥KID+公钥文件
#define ECCPRI_PUB "0x11:serverpub.pem"

//服务端证书文件
#define SEVER_CRT_FILE  "server.crt"
//CA证书
#define CA_CRT_FILE "ca.crt"
//设置端口IP
#define DEFAULT_IP              "127.0.0.1"
#define DEFAULT_PORT            5000
//设置安全传输模式
#define SECURE_COMM		TLS_server_method()
//#define SECURE_COMM		DTLS_server_method()

typedef enum {
	SOCKET_IS_NONBLOCKING,
	SOCKET_IS_BLOCKING,
	SOCKET_HAS_TIMED_OUT,
	SOCKET_HAS_BEEN_CLOSED,
	SOCKET_OPERATION_OK
} timeout_state;

extern  int waitpid();

void serverListen(void);
void SSLConnected(int,int);

int main (int argc, char *argv[])
{

	
  serverListen();

	return 0;
}

void serverListen(void)
{
	int                     err;
	int                     error=0;
	int                     pid;
	int                     listen_sock;
	int                     sock;
	struct sockaddr_in      sa_serv;
	struct sockaddr_in      sa_cli;
	size_t                  client_len;
	short int               s_port = DEFAULT_PORT;
	int                     connect=0;


   port_printf("\n+++++++++++++++++++++++++++++++\n");
	// 设置Socket
	listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (listen_sock == -1)
	{
		error=1;
	}
	memset(&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family = AF_INET;
	sa_serv.sin_addr.s_addr = INADDR_ANY;
	sa_serv.sin_port = htons(s_port);
	err = bind(listen_sock, (struct sockaddr*)&sa_serv,sizeof(sa_serv));
	if (err == -1)
	{
		error=1;
	}

	/*等待TCP连接*/
	err = listen(listen_sock, 5);
	if (err == -1)
	{
		error=1;
	}
	client_len = sizeof(sa_cli);

	while (error == 0)
	{
    port_printf("\nListening to incoming connection\n");
		sock = accept(listen_sock, (struct sockaddr*)&sa_cli,(socklen_t *) &client_len);
		if (sock == -1)
		{
			error=1;
			break;
		}

			port_printf("Connection from %d.%d.%d.%d, port :0x%.4x",
				sa_cli.sin_addr.s_addr & 0x000000ff,
				(sa_cli.sin_addr.s_addr & 0x0000ff00) >> 8,
				(sa_cli.sin_addr.s_addr & 0x00ff0000) >> (8*2),
				(sa_cli.sin_addr.s_addr & 0xff000000) >> (8*3),
				sa_cli.sin_port);
		
		pid = fork();
		if (pid == -1)
		{
		    error=1;
		}

		if (pid == 0)
		{
			connect = getpid();
			close(listen_sock);
			SSLConnected(sock,connect);
      port_printf("\n[%d] Return from Child", connect);
			error=1;
		}
	}
	// 关闭socket
	err = close(sock);
	if (err == -1)
	{
		error=1;
	}
}

void SSLConnected(int sock, int connect)
{
	int             err;
	int             len;
	int             error=0;
	clock_t		start, end;
	SSL_CTX         *ctx;
	SSL             *ssl;
	SSL_METHOD      *meth;
	char         buf[4096];
	ENGINE          *e;
	EVP_PKEY        *pkey;
	UI_METHOD       *ui_method;
	EC_KEY *ecdh;

	do {
	    // 初始化penssl
	    SSL_library_init();
	    SSL_load_error_strings();
	   	meth = (SSL_METHOD*) SECURE_COMM;
      
      //产生一个SL_CTX
	    ctx = SSL_CTX_new(meth);    
	    if (!ctx)
	    {
    		ERR_print_errors_fp(stderr);
    		exit(1);
	    }

	    ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	    SSL_CTX_set_tmp_ecdh(ctx,ecdh);
     
      
	    //加载和初始化引擎
	    ENGINE_load_builtin_engines();
	    e = ENGINE_by_id(ENGINE_NAME);
	    if(!e)
	    {
	    	port_printf("loading Engine failed!!\n");
	    }
	    port_printf("Engine ID : %s\n",ENGINE_get_id(e));

	    if(!ENGINE_init(e))
	    {
	   	  port_printf("Init hed Engine failed!!\n");
	    }
	    port_printf("hed engine init Ok\n");

	    if(!ENGINE_set_default(e, ENGINE_METHOD_ALL))
	    {
	    	port_printf("Hed Engine failede!\n");
	    }
	    port_printf("hed engine setting Ok\n");
        
	    //加载私钥
	    ui_method = UI_OpenSSL();
	    pkey = ENGINE_load_private_key(e,ECCPRI_PUB,ui_method,NULL);
	   //SSL_CTX_use_PrivateKey_file(ctx, pkey, SSL_FILETYPE_PEM);
     SSL_CTX_use_PrivateKey(ctx, pkey);
	   
     //加载证书 
	    if(SSL_CTX_use_certificate_file(ctx,SEVER_CRT_FILE, SSL_FILETYPE_PEM) <= 0)
	    {
		   port_printf("Load Certificate Fail\n");
		   break;
	    }
	    port_printf("Load Certificate ok\n");

	    // 检查私钥和证书是否匹配
	   if(!SSL_CTX_check_private_key(ctx))
	   {
    		port_printf("Private Key do not Match the Server Certificate!!!!\n");
    		break;
	   }
	    port_printf("Private Key Match the Server Certificate.\n");

	   //加载CA证书
	   if(!SSL_CTX_load_verify_locations(ctx, CA_CRT_FILE, NULL))
	   {
    		port_printf("Load CA cert Fail\n");
    		break;
	   }
	      port_printf("Load CA cert ok\n");
 
	  
	   SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

	   SSL_CTX_set_verify_depth(ctx,1);
	}while(0);

    if(error==0)
    {
	// 产生一个新的SSL
	ssl = SSL_new(ctx);
	if (ssl == NULL)
	{
	    error = 1;
	}

	// 将socket加入到SSL
	SSL_set_fd(ssl, sock);
	// 建立SSL连接
  port_printf("waiting for the client connection!");
	err = SSL_accept(ssl);
  
	switch(err)
	{
		case 1:
			port_printf("The cipher suite is : %s\n", SSL_get_cipher(ssl));
			port_printf("The SSL/TLS version is : %s\n", SSL_get_version(ssl));
			start = clock();
			while(1)
			{
				len = SSL_read(ssl, buf, sizeof(buf) - 1);
				err = SSL_get_error(ssl, len);

				if ((err != 0) && (len == 0))
				{
				    end = clock();
				    if (((end-start)/1000000) > 5)
				    {
					port_printf("[%d] Timeout !!\n",connect);
					break;
				    }
				}
				else
				{
					start = clock();
					port_printf("[%d] the data from client is : %d\n", connect, buf[0]);
					if (buf[0] > 100)
						break;

					sprintf(buf,"From Server [%d] : %.3d",connect, buf[0]);
					err = SSL_write(ssl, buf,strlen(buf));
					if (err==-1)
					{
						ERR_print_errors_fp(stderr);
					}
				}
			}
			break;
		case 0:
			port_printf("Connection Refuse\n ");
			break;
		default:
			port_printf("SSL Error!!! %d\n",err);
	}

  //关闭SSL连接
	err = SSL_shutdown(ssl);
	port_printf("Connection Closed!!!\n");
    }

    //释放SSL 
    ENGINE_free(e);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    port_printf("Leaving Routine!!!\n");
}
