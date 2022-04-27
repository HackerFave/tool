#ifndef CLIENT_SSL_H
#define CLIENT_SSL_H
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <iostream>
#include <pthread.h>
#include <thread>
using namespace std;
#define MAXBUF 1024


class client_ssl
{
public:
    struct testData{
        int num;
        char str[100];
        bool checked;
    };
    struct mypara
    {
        void* para1;//参数1
        int para2;//参数2
    };

    client_ssl(const std::string &ca,const std::string &client_cert, const std::string & privateKey,const std::string & ip,const std::string & port);
    ~client_ssl();
    void init();
private:
    bool initSSL();
    void closeSSL();
    bool checkCerts(SSL * ssl);
    bool readData(void *buf, int num);
    bool writeData(const void *buf, int num);
    static void threadRead(int );
    static void threadWrite(int);
private:
    static client_ssl *p_client_ssl;
    int _sockfd/*, _len*/;
    struct sockaddr_in _scokrt_dest;
    SSL_CTX *_ssl_ctx;
    SSL *_ssl;
    testData _testData;
    std::string _ca;
    std::string _client_cert;
    std::string _privateKey;
    std::string _ip;
    std::string _port;
};

#endif // CLIENT_SSL_H

