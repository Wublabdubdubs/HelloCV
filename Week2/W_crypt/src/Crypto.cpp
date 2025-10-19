#include<string>
#include "../include/Crypto.h"

void Crypto::KS_Work(std::string S,int Key,int opt,std::string &T)
{
    T=S;
    for(int i=0;i<T.size();i++)
    {
        if(T[i]>='A'&&T[i]<='Z')
        {
            if(opt==1)
                T[i]=(int(T[i]-'A')+Key)%26+'A';
            else 
                T[i]=(int((T[i]-'A')-Key)%26+26)%26+'A';
        } 
        else if(T[i]>='a'&&T[i]<='z')
        {
            if(opt==1)
                T[i]=(int(T[i]-'a')+Key)%26+'a';
            else 
                T[i]=(int((T[i]-'a')-Key)%26+26)%26+'a';
        }
    }
    return ;
}

void Crypto::XOR_Work(std::string S,std::string Key,int opt,std::string &T)
{
    while(Key.size()<S.size()) Key=Key+Key;
    for(int i=0;i<S.size();i++) T.push_back(S[i]^Key[i]);
    return ;
}