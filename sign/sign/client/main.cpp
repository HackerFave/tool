#include <QCoreApplication>
//#include <stdio.h>
//#include <string.h>
//#include <errno.h>
//#include <sys/socket.h>
//#include <resolv.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>

//#define MAXBUF 1024

//void ShowCerts(SSL * ssl)
//{
//  X509 *cert;
//  char *line;

//  cert = SSL_get_peer_certificate(ssl);
//  if (cert != NULL) {
//    printf("Digital certificate information:\n");
//    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
//    printf("Certificate: %s\n", line);
//    free(line);
//    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
//    printf("Issuer: %s\n", line);
//    free(line);
//    X509_free(cert);
//  }
//  else
//    printf("No certificate information！\n");
//}

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    int i,j,sockfd, len, fd, size;
//    char fileName[50],sendFN[20];
//    struct sockaddr_in dest;
//    char buffer[MAXBUF + 1];
//    SSL_CTX *ctx;
//    SSL *ssl;

//    if (argc != 3)
//    {
//      printf("Parameter format error! Correct usage is as follows：\n\t\t%s IP Port\n\tSuch as:\t%s 127.0.0.1 80\n", argv[0], argv[0]); exit(0);
//    }

//    /* SSL 库初始化 */
//    SSL_library_init();
//    OpenSSL_add_all_algorithms();
//    SSL_load_error_strings();
//    ctx = SSL_CTX_new(SSLv23_client_method());
//    if (ctx == NULL)
//    {
//      ERR_print_errors_fp(stdout);
//      exit(1);
//    }

//    /* 建立一个 socket 用于 tcp 通讯 */
//    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//    {
//      perror("Socket");
//      exit(errno);
//    }
//    printf("socket created\n");

//    /* 初始化服务器端（对方）的地址和端口信息 */
//    bzero(&dest, sizeof(dest));
//    dest.sin_family = AF_INET;
//    dest.sin_port = htons(atoi(argv[2]));
//    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0)
//    {
//      perror(argv[1]);
//      exit(errno);
//    }
//    printf("address created\n");

//    /* 链接服务器 */
//    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0)
//    {
//      perror("Connect ");
//      exit(errno);
//    }
//    printf("server connected\n\n");

//    /* 基于 ctx 产生一个新的 SSL */
//    ssl = SSL_new(ctx);
//    SSL_set_fd(ssl, sockfd);
//    /* 创建 SSL 链接 */
//    if (SSL_connect(ssl) == -1)
//      ERR_print_errors_fp(stderr);
//    else
//    {
//      printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
//      ShowCerts(ssl);
//    }

//    /* 接收用户输入的文件名，并打开文件 */
//    printf("\nPlease input the filename of you want to load :\n>");
//    scanf("%s",fileName);
//    if((fd = open(fileName,O_RDONLY,0666))<0)
//    {
//      perror("open:");
//      exit(1);
//    }

//    /* 将用户输入的文件名，去掉路径信息后，发给服务器 */
//    for(i=0;i<=strlen(fileName);i++)
//    {
//      if(fileName[i]=='/')
//      {
//        j=0;
//        continue;
//      }
//      else {sendFN[j]=fileName[i];++j;}
//    }
//    len = SSL_write(ssl, sendFN, strlen(sendFN));
//    if (len < 0)
//      printf("'%s'message Send failure ！Error code is %d，Error messages are '%s'\n", buffer, errno, strerror(errno));

//    /* 循环发送文件内容到服务器 */
//    bzero(buffer, MAXBUF + 1);
//    while((size=read(fd,buffer,1024)))
//    {
//      if(size<0)
//      {
//        perror("read:");
//        exit(1);
//      }
//      else
//      {
//        len = SSL_write(ssl, buffer, size);
//        if (len < 0)
//          printf("'%s'message Send failure ！Error code is %d，Error messages are '%s'\n", buffer, errno, strerror(errno));
//      }
//      bzero(buffer, MAXBUF + 1);
//    }
//    printf("Send complete !\n");

//    /* 关闭链接 */
//    close(fd);
//    SSL_shutdown(ssl);
//    SSL_free(ssl);
//    close(sockfd);
//    SSL_CTX_free(ctx);
//    return a.exec();
//}

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
using namespace std;
#define FAIL    -1
int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    if ( (host = gethostbyname(hostname)) == NULL )
    {
//        printf('Eroor: %sn',hostname);
        std::cout<<"Error"<<hostname<<std::endl;
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
    method = SSLv2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        printf('Eroor: %sn',stderr);
        abort();
    }
    return ctx;
}
void ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %sn", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %sn", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("No certificates.n");
}
int main(int count, char *strings[])
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    int bytes;
    char *hostname, *portnum;
    if ( count != 3 )
    {
        printf("usage: %s <hostname> <portnum>n", strings[0]);
        exit(0);
    }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];
    ctx = InitCTX();
    server = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);      /* create new SSL connection state */
    SSL_set_fd(ssl, server);    /* attach the socket descriptor */
    if ( SSL_connect(ssl) == FAIL )   /* perform the connection */
    {
        printf('Eroor: %sn',stderr);
        ERR_print_errors_fp(stderr);
    }
    else
    {   char *msg = "HelloWorld";
        printf("Connected with %s encryptionn", SSL_get_cipher(ssl));
        ShowCerts(ssl);        /* get any certs */
        SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
        buf[bytes] = 0;
        printf("Received: "%s"n", buf);
        SSL_free(ssl);        /* release connection state */
    }
    close(server);         /* close socket */
    SSL_CTX_free(ctx);        /* release context */
    return 0;
}
