#ifndef VERIFY_H
#define VERIFY_H
#include "tools.h"

class verify
{
public:
    verify();
    char* rsa_sign(const char* privateKey,const char* content);
    bool rsa_verify(const char* publicKey,const char* sign,const char* content);
    char* EncryData(const char* publicKey,const char* content);
    void DecryData(const char* privateKey,const char* content);


};

#endif // VERIFY_H
