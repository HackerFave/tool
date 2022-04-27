#include <QCoreApplication>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pkcs12.h>
#include <openssl/pem.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#define IP "127.0.0.1"
//#define PORT 4433
//#define CERT_PATH "./sslclientcert.pem"
//#define KEY_PATH  "./sslclientkey.pem"
//#define CAFILE "./demoCA/cacert.pem"
//static SSL_CTX *g_sslctx = NULL;


//int connect_to_server(int fd ,char* ip,int port){
//    struct sockaddr_in svr;
//    memset(&svr,0,sizeof(svr));
//    svr.sin_family = AF_INET;
//    svr.sin_port = htons(port);
//    if(inet_pton(AF_INET,ip,&svr.sin_addr) <= 0){
//        printf("invalid ip address!\n");
//        return -1;
//    }
//    if(connect(fd,(struct sockaddr *)&svr,sizeof(svr))){
//        printf("connect error : %s\n",strerror(errno));
//        return -1;
//    }

//    return 0;
//}

////客户端证书内容输出
//void print_client_cert(char* path)
//{
//    X509 *cert =NULL;
//    FILE *fp = NULL;
//    fp = fopen(path,"rb");
//    //从证书文件中读取证书到x509结构中，passwd为1111,此为生成证书时设置的
//    char *str = "1111";
//    cert = PEM_read_X509(fp, NULL, NULL,str);
//    X509_NAME *name=NULL;
//    char buf[8192]={0};
//    BIO *bio_cert = NULL;
//    //证书持有者信息
//    name = X509_get_subject_name(cert);
//    X509_NAME_oneline(name,buf,8191);
//    printf("ClientSubjectName:%s\n",buf);
//    memset(buf,0,sizeof(buf));
//    bio_cert = BIO_new(BIO_s_mem());
//    PEM_write_bio_X509(bio_cert, cert);
//    //证书内容
//    BIO_read( bio_cert, buf, 8191);
//    printf("CLIENT CERT:\n%s\n",buf);
//    if(bio_cert)BIO_free(bio_cert);
//    fclose(fp);
//    if(cert) X509_free(cert);
//}
////在SSL握手时，验证服务端证书时会被调用，res返回值为1则表示验证成功，不然为失败
//static int verify_cb(int res, X509_STORE_CTX *xs)
//{
//    printf("SSL VERIFY RESULT :%d\n",res);
////    switch (xs->error)
////    {
////        case X509_V_ERR_UNABLE_TO_GET_CRL:
////            printf(" NOT GET CRL!\n");
////            return 1;
////        default :
////            break;
////    }
//    return res;
//}

//int sslctx_init()
//{
//#if 0
//    BIO *bio = NULL;
//    X509 *cert = NULL;
//    STACK_OF(X509) *ca = NULL;
//    EVP_PKEY *pkey =NULL;
//    PKCS12* p12 = NULL;
//    X509_STORE *store =NULL;
//    int error_code =0;
//#endif

//    int ret =0;
//    print_client_cert(CERT_PATH);
//    //registers the libssl error strings
//    SSL_load_error_strings();

//    //registers the available SSL/TLS ciphers and digests
//    SSL_library_init();

//    //creates a new SSL_CTX object as framework to establish TLS/SSL
//    g_sslctx = SSL_CTX_new(SSLv23_client_method());
//    if(g_sslctx == NULL){
//        ret = -1;
////        goto end;
//    }

//    //passwd is supplied to protect the private key,when you want to read key
//    char * str = "1111";
//    SSL_CTX_set_default_passwd_cb_userdata(g_sslctx,str);

//    //set cipher ,when handshake client will send the cipher list to server
//    SSL_CTX_set_cipher_list(g_sslctx,"HIGH:MEDIA:LOW:!DH");
//    //SSL_CTX_set_cipher_list(g_sslctx,"AES128-SHA");

//    //set verify ,when recive the server certificate and verify it
//    //and verify_cb function will deal the result of verification
//    SSL_CTX_set_verify(g_sslctx, SSL_VERIFY_PEER, verify_cb);

//    //sets the maximum depth for the certificate chain verification that shall
//    //be allowed for ctx
//    SSL_CTX_set_verify_depth(g_sslctx, 10);

//    //load the certificate for verify server certificate, CA file usually load
//    SSL_CTX_load_verify_locations(g_sslctx,CAFILE, NULL);

//    //load user certificate,this cert will be send to server for server verify
//    if(SSL_CTX_use_certificate_file(g_sslctx,CERT_PATH,SSL_FILETYPE_PEM) <= 0){
//        printf("certificate file error!\n");
//        ret = -1;
//        goto end;
//    }
//    //load user private key
//    if(SSL_CTX_use_PrivateKey_file(g_sslctx,KEY_PATH,SSL_FILETYPE_PEM) <= 0){
//        printf("privatekey file error!\n");
//        ret = -1;
//        goto end;
//    }
//    if(!SSL_CTX_check_private_key(g_sslctx)){
//        printf("Check private key failed!\n");
//        ret = -1;
//        goto end;
//    }

//end:
//    return ret;
//}

//void sslctx_release()
//{
//    EVP_cleanup();
//    if(g_sslctx){
//        SSL_CTX_free(g_sslctx);
//    }
//    g_sslctx= NULL;
//}
////打印服务端证书相关内容
//void print_peer_certificate(SSL *ssl)
//{
//    X509* cert= NULL;
//    X509_NAME *name=NULL;
//    char buf[8192]={0};
//    BIO *bio_cert = NULL;
//    //获取server端证书
//    cert = SSL_get_peer_certificate(ssl);
//    //获取证书拥有者信息
//    name = X509_get_subject_name(cert);
//    X509_NAME_oneline(name,buf,8191);
//    printf("ServerSubjectName:%s\n",buf);
//    memset(buf,0,sizeof(buf));
//    bio_cert = BIO_new(BIO_s_mem());
//    PEM_write_bio_X509(bio_cert, cert);
//    BIO_read( bio_cert, buf, 8191);
//    //server证书内容
//    printf("SERVER CERT:\n%s\n",buf);
//    if(bio_cert)BIO_free(bio_cert);
//    if(cert)X509_free(cert);
//}

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//    int fd = -1 ,ret = 0;
//    SSL *ssl = NULL;
//    char buf[1024] ={0};
//    //初始化SSL
//    if(sslctx_init()){
//        printf("sslctx init failed!\n");
//        goto out;
//    }
//    //客户端socket创建tcp链接
//    fd = socket(AF_INET,SOCK_STREAM,0);
//    if(fd < 0){
//        printf("socket error:%s\n",strerror(errno));
//        goto out;
//    }

//    if(connect_to_server(fd ,IP,PORT)){
//        printf("can't connect to server:%s:%d\n",IP,PORT);
//        goto out;
//    }
//    ssl = SSL_new(g_sslctx);
//    if(!ssl){
//        printf("can't get ssl from ctx!\n");
//        goto out;
//    }
//    SSL_set_fd(ssl,fd);
//    //创建与服务端SSL链接
//    ret = SSL_connect(ssl);
//    if(ret != 1){
//        int err = ERR_get_error();
//        printf("Connect error code: %d ,string: %s\n",err,ERR_error_string(err,NULL));
//        goto out;
//    }
//    //输入服务端证书内容
//    print_peer_certificate(ssl);

//    //SSL_write(ssl,"sslclient test!",strlen("sslclient test!"));
//    //SSL_read(ssl,buf,1024);
//    //关闭SSL链接
//    SSL_shutdown(ssl);


//out:
//    if(fd >0)close(fd);
//    if(ssl != NULL){
//        SSL_free(ssl);
//        ssl = NULL;
//    }
//    if(g_sslctx != NULL) sslctx_release();
//    return a.exec();
//}
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>

int https_get_request_wait_rsp(const char *ip, const char *name, int portnumber, const char *get_str, char *rsp_str, int rsp_buf_len)
{
    int sockfd = 0;
    int ret;
    char buffer[1024*1024];
    int nbytes;
    char host_addr[256];
    char request[1024];
    int send, totalsend;
    int i;
    SSL *ssl;
    SSL_CTX *ctx;
    char server_ip[16] = {0};
    struct hostent *host;
    struct in_addr addr;
    struct sockaddr_in servaddr;

    if(ip == NULL || strlen(ip) < 7)
    {
        if((host = gethostbyname(name)) == NULL )
        {
            return -1;
       }
       memcpy(&addr.s_addr,host->h_addr_list[0],sizeof(addr.s_addr));
       strcpy(server_ip,(char *)inet_ntoa(addr));
    }
    else
    {
        strcpy(server_ip, ip);
    }

    /* 客户程序开始建立 sockfd描述符 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {      /*建立SOCKET连接 */
        printf("socket fail..............\n");
        return -1;

    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(portnumber);
    if(inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", server_ip);
        close(sockfd);
        return -1;
    }
     /* 客户程序发起连接请求 */
    if (connect(sockfd, (struct sockaddr *) (&servaddr), sizeof(struct sockaddr)) == -1)
    {
        printf( "Connect Error:%s\a\n", strerror(errno));
        close(sockfd);
        return -1;
    }

    /*ssl init*/
    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if(ctx == NULL)
    {
        close(sockfd);
        return -1;
    }

    ssl = SSL_new(ctx);
    if(ssl == NULL)
    {
        close(sockfd);
        return -1;
    }

    /*把socket和SSL关联*/
    ret = SSL_set_fd(ssl, sockfd);
    if(ret == 0)
    {
        close(sockfd);
        return -1;
    }

    ret = SSL_connect(ssl);
    if(ret != 1)
    {
        close(sockfd);
        return -1;
    }
    sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\nHost: %s:%d\r\nConnection: Keep-alive\r\n\r\n", get_str, server_ip, portnumber);

    printf("%s", request);


    send = 0;
    totalsend = 0;
    nbytes = strlen(request);
    while(totalsend < nbytes)
    {
        send = SSL_write(ssl, request , nbytes - totalsend);
        if(send == -1)
        {
            close(sockfd);
            return -1;
        }
        totalsend  = send;
    }
    printf("44444444:%d\n", totalsend);
    i = 0;

    /*
    while((nbytes = SSL_read(ssl, buffer, 1)) == 1)
    {
        printf("buffer:%s\n", buffer);
        if(i < 4)
        {
            if(buffer[0] == 'r' || buffer[0] == '\n')
            {
                i  ;
                if(i>=4)
                {
                    break;
                }

            }
            else
            {
                i = 0;
            }
        }
    }*/
    printf("5555555\n");

    memset(rsp_str, 0, rsp_buf_len);
    ret = SSL_read(ssl, rsp_str, rsp_buf_len);
    if(ret < 0)
    {
        printf("response ret =%d ======\n", ret);
        close(sockfd);
        return -1;
    }
    printf("response ret =%d=====>\t\t[%s]\n\n", ret , rsp_str);


    /*end ssl*/
    SSL_shutdown(ssl);
    close(sockfd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();

    return 0;
}
int main(void)
{

    //char *hostname, *portnum, *weburl;

    char rsp_buf[40960] = {0};
    https_get_request_wait_rsp(NULL, "www.baidu.com", 443, "", rsp_buf, sizeof(rsp_buf));

    printf("=======>\n");


    return 0;
}
