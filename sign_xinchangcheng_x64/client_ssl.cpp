#include "client_ssl.h"
client_ssl * client_ssl:: p_client_ssl = nullptr;
client_ssl::client_ssl(const std::string &ca,const std::string &client_cert,
                       const std::string & privateKey,const std::string & ip,const std::string & port):
    _ca(ca),
    _client_cert(client_cert),
    _privateKey(privateKey),
    _ip(ip),
    _port(port)
{
    p_client_ssl = this;
}
void client_ssl::init()
{
    if(initSSL())
    {

        std::cout << "Spawning 5 threads...\n";
        std::thread threads[2];
        threads[0] = std::thread(threadRead, 0 );
        threads[1] = std::thread(threadWrite, 1 );
        std::cout << "Done spawning threads! Now wait for them to join\n";
        for (auto& t: threads) {
            t.join();
        }
        std::cout << "All threads joined.\n";


    }
}
client_ssl::~client_ssl()
{
    closeSSL();
}
bool client_ssl::initSSL()
{
    /* SSL 库初始化 */
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    _ssl_ctx = SSL_CTX_new(SSLv23_client_method());
    if (_ssl_ctx == NULL) {
        ERR_print_errors_fp(stdout);
        return false;
    }

    // 双向验证
    // SSL_VERIFY_PEER---要求对证书进行认证，没有证书也会放行
    // SSL_VERIFY_FAIL_IF_NO_PEER_CERT---要求客户端需要提供证书，但验证发现单独使用没有证书也会放行
    SSL_CTX_set_verify(_ssl_ctx, SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
    // 设置信任根证书
    if (SSL_CTX_load_verify_locations(_ssl_ctx, _ca.c_str(),NULL)<=0){
        ERR_print_errors_fp(stdout);
        return false;
    }

    /* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
    if (SSL_CTX_use_certificate_file(_ssl_ctx, _client_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        return false;
    }
    /* 载入用户私钥 */
    if (SSL_CTX_use_PrivateKey_file(_ssl_ctx, _privateKey.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stdout);
        return false;
    }
    /* 检查用户私钥是否正确 */
    if (!SSL_CTX_check_private_key(_ssl_ctx)) {
        ERR_print_errors_fp(stdout);
        return false;
    }

    /* 创建一个 socket 用于 tcp 通信 */
    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(errno);
    }
    printf("socket created\n");

    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero(&_scokrt_dest, sizeof(_scokrt_dest));
    _scokrt_dest.sin_family = AF_INET;
    _scokrt_dest.sin_port = htons(atoi(_port.c_str()));
    if (inet_aton(_ip.c_str(), (struct in_addr *) &_scokrt_dest.sin_addr.s_addr) == 0) {
        perror(_ip.c_str());
        //        exit(errno);
        return false;
    }
    printf("address created\n");

    /* 连接服务器 */
    if (connect(_sockfd, (struct sockaddr *) &_scokrt_dest, sizeof(_scokrt_dest)) != 0) {
        perror("Connect ");
        //        exit(errno);
        return false;
    }
    printf("server connected\n");

    /* 基于 ctx 产生一个新的 SSL */
    _ssl = SSL_new(_ssl_ctx);
    SSL_set_fd(_ssl, _sockfd);
    /* 建立 SSL 连接 */
    if (SSL_connect(_ssl) == -1){
        ERR_print_errors_fp(stderr);
        return false;
    }
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(_ssl));
        if(checkCerts(_ssl)){
            return true;
        }
        return false;
    }
    return true;
}
//关闭连接
void client_ssl::closeSSL()
{
    /* 关闭连接 */
    SSL_shutdown(_ssl);
    SSL_free(_ssl);
    close(_sockfd);
    SSL_CTX_free(_ssl_ctx);
}
bool client_ssl::checkCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    // SSL_get_verify_result()是重点，SSL_CTX_set_verify()只是配置启不启用并没有执行认证，调用该函数才会真证进行证书认证
    // 如果验证不通过，那么程序抛出异常中止连接
    if(SSL_get_verify_result(ssl) == X509_V_OK){
        printf("证书验证通过\n");

    }
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
        return true;
    } else{
        printf("无证书信息！\n");
        return false;
    }
}
bool client_ssl::readData(void *buf, int num)
{

    /* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */
    //    bzero(_buffer, MAXBUF + 1);
    int len;
    /* 接收服务器来的消息 */
    len = SSL_read(_ssl, buf, num);
    if (len > 0){/*
        printf("接收消息成功:'%s'，共%d个字节的数据\n",
               buf, len);*/
        return true;
    }
    else {/*
        printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",
               errno, strerror(errno));*/
        return false;
    }
}
bool client_ssl::writeData(const void *buf, int num)
{
    int len;
    //    /* 发消息给服务器 */
    len = SSL_write(_ssl, &buf, num);
    if (len < 0){/*
        printf("消息'%s'发送失败！错误代码是%d，错误信息是'%s'\n",
               buf, errno, strerror(errno));*/
        return false;
    }
    else{/*
        printf("消息'%s'发送成功，共发送了%d个字节！\n",
               &_testData, sizeof(struct testData));*/
        return true;
    }

}
void  client_ssl::threadRead(int para)
{
    //    mypara *pstru;
    //    pstru = (struct mypara *) para;
    static int i =0;
    while(1){
        char buffer[MAXBUF + 1];
        if( p_client_ssl->readData(buffer,MAXBUF)){
            std::cout<<"[接收消息：]"<<(string)buffer<<"num"<<i++<<std::endl;
        }
        sleep(3);
    }
}
void client_ssl::threadWrite(int para)
{
    //    mypara *pstru;
    //    pstru = (struct mypara *) para;
    //    p_client_ssl->writeData(pstru->para1,pstru->para2);
    static testData test;
    static int i =0;
    while (1) {
        test.num = 1;
        strcpy(test.str,"我在测试数据");
        test.checked = true;
        if(p_client_ssl->writeData(&test,sizeof (struct testData))){
            std::cout<<"[发送消息成功：]"<<test.str<<"num"<<i++<<std::endl;
        }
        sleep(3);
    }

}
