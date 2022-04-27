#include "verify.h"
#include "tool.h"
#include<string.h>
verify::verify()
{

}


char * verify::rsa_sign(const char *privateKey, const char *content)
{
    char p_sign[10000] = {0};
    RSA *p_rsa = GetKeyRSA(privateKey,2);
    int rsa_len = RSA_size(p_rsa);
    if(RSA_sign(NID_md5,(unsigned char*)content,strlen(content),(unsigned char*)p_sign,(unsigned int *)&rsa_len,p_rsa)<0){
        cout<<"RSA_SIGN FAILED"<<endl;
        return "";
    }
    RSA_free(p_rsa);
    char *sign = base64Encode(p_sign,rsa_len);
    cout<<"sign===="<<sign<<endl;
    return sign;


}

bool verify::rsa_verify(const char *publicKey, const char *sign, const char *content)
{

    char p_ver[1000] = {0};
    RSA *p_rsa = GetKeyRSA(publicKey,1);
    int rsa_len = RSA_size(p_rsa);
    std::string dSign = base64_decode(sign);
    int res = RSA_verify(NID_md5,(unsigned char*)content,strlen(content) ,(unsigned char*)dSign.c_str(),dSign.length() ,p_rsa);
    RSA_free(p_rsa);
    return res == 1;
}

char* verify::EncryData(const char *publicKey, const char *content)
{

    RSA *p_rsa = GetKeyRSA(publicKey,1);
    int rsa_len = RSA_size(p_rsa);
    char p_e[1000] = {0};

    if(RSA_public_encrypt(strlen(content), (unsigned char *)content, (unsigned char*)p_e, p_rsa, RSA_PKCS1_PADDING)<0) {
        cout<<"RSA_public_encrypt err"<<endl;
        return NULL;
    }

    while (true) {
        if(strlen(p_e) == 128){
            break;
        }
        if(RSA_public_encrypt(strlen(content), (unsigned char *)content, (unsigned char*)p_e, p_rsa, RSA_PKCS1_PADDING)<0)
            cout<<"RSA_public_encrypt err"<<endl;

    }
    RSA_free(p_rsa);
    char * bp_e = base64Encode(p_e,rsa_len);
    cout<<"encrydate====="<<bp_e<<endl;
    return bp_e;

}

void verify::DecryData(const char *privateKey, const char *content)
{

    RSA *p_rsa = GetKeyRSA(privateKey,2);
    int rsa_len = RSA_size(p_rsa);
    char p_de[1000] = {0};
    std::string unBase64 = base64_decode(content);
    if (RSA_private_decrypt(unBase64.length(), (unsigned char *)unBase64.c_str(), (unsigned char*)p_de, p_rsa, RSA_PKCS1_PADDING)<0) {//RSA_PKCS1_PADDING
        cout<<"RSA_private_decrypt err"<<endl;
        return ;
    }
    RSA_free(p_rsa);
    cout<<"RSA_private_decrypt:"<<p_de<<endl;
}

