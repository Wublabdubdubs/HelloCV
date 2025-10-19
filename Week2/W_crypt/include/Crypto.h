#ifndef CRYPTO_TAG
#define CRYPTO_TAG
#include<string>
class Crypto
{
    public:
        void KS_Work(std::string S,int Key,int opt,std::string &T);
        void XOR_Work(std::string S,std::string Key,int opt,std::string &T);
    private:
};
#endif