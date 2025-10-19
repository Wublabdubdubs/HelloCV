#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include "../include/Menu.h"
#include "../include/Crypto.h"
void Menu::Hello()
{
    printf(
        "---------------------分割线--------------------\n"
        "欢迎使用文本加密/解密工具 W_crypt ^_^\n"
        "使用中遇到任何问题欢迎反馈 QQ:386433875\n"
        "-----------------------------------------------\n"
    );
    return ;
}

void Menu::Error(int op=1)
{
    if(op==1) 
        printf("抱歉，未找到您输入的选项，请重试\n");

    return ;
}

void Menu::End()
{
    printf("程序即将关闭，感谢您的使用\n");
    return ;
}

void Menu::Work()
{
    Crypto Solve;
Choose_IO_Pre:
    printf(
        "请输入序号选择你想要的交互模式：\n"
        "1. 命令行运行\n"
        "2. 文件输入输出\n"
    );
    int opt_IO,opt_work,opt_type;
Choose_IO:
    scanf("%d",&opt_IO);
    if(opt_IO>0&&opt_IO<=2)
    {
    Choose_Type_Pre:
        printf("您已选择");printf(opt_IO==1?"命令行":"文件");printf("交互，请选择加密类型：\n");
        printf(
            "0. 返回上级目录\n"
            "1. 凯撒密码\n"
            "2. XOR加密\n"
        );
    Choose_Type:
        scanf("%d",&opt_type);
        if(opt_type==0) goto Choose_IO_Pre;
        else if(opt_type<0||opt_type>2)  
        {
            this->Error();
            goto Choose_Type; 
        }
    Start_Work:
        printf("您已选择");printf(opt_type==1?"凯撒密码":"XOR加密");printf("，您希望：\n");
        printf(
            "0. 返回上级目录\n"
            "1. 加密文本\n"
            "2. 解密文本\n"
        );
        std::cin>>opt_work;
        
        if(opt_work==0) goto Choose_Type_Pre;
        else if(opt_IO==1)
        {
            std::string S,T;
            int KS_Key;
            std::string XOR_Key;
            getline(std::cin,S);
            printf("请输入文本：\n");
            getline(std::cin,S);
            printf("请输入密钥：\n");
            if(opt_type==1)
            {
                std::cin>>KS_Key;
                Solve.KS_Work(S,KS_Key,opt_work,T);
            }
            else
            {
                getline(std::cin,XOR_Key);
                Solve.XOR_Work(S,XOR_Key,opt_work,T);
            }
            printf("结果为：\n");
            std::cout<<T<<std::endl;
        }
        else if(opt_IO==2)
        {
            std::ifstream F_in;
            std::ofstream F_out;
            std::string Path_in,Path_out,S,XOR_Key,T;
            int KS_Key;
            getline(std::cin,S);
            printf("请输入输入文件路径：\n");
            getline(std::cin,Path_in);
            if(Path_in=="") Path_in="../bin/in.dat";
            printf("请输入输出文件路径：\n");
            getline(std::cin,Path_out);
            if(Path_out=="") Path_out="../bin/out.dat";
            F_in.open(Path_in,std::ios::in);
            F_out.open(Path_out,std::ios::out);
            if(opt_type==1)
            {

                getline(F_in,S);
                F_in>>KS_Key;
                Solve.KS_Work(S,KS_Key,opt_work,T);
            }
            else
            {

                getline(F_in,S);
                getline(F_in,XOR_Key);
                Solve.XOR_Work(S,XOR_Key,opt_work,T);
            }
            F_out<<T<<std::endl;
            F_in.close();
            F_out.close();
            // F_in.open()
        }
        printf(
            "运行已完成，您希望：\n"
            "1. 保持上述参数再次运行\n"
            "2. 更换参数再次运行\n"
            "输入任意其他整数推出\n"
        );
        int tmp;std::cin>>tmp;
        if(tmp==1) goto Start_Work;
        else if(tmp==2) goto Choose_IO_Pre;
    }
    else
    {
        this->Error();
        goto Choose_IO;
    }
        
    return ;
}