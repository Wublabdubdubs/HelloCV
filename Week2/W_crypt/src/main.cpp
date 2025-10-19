#include<iostream>
#include<fstream>
using namespace std;
#include "../include/Crypto.h"
#include "../include/Menu.h"


Menu Main_Menu;

int main()
{
    Main_Menu.Hello();
    Main_Menu.Work();
    Main_Menu.End();
    // test
    // std::ifstream F_in;
    // std::ofstream F_out;
    // std::string s,sin="../bin/in.dat";
    // std::string sout="../bin/out.dat";
    // F_in.open(sin,std::ios::in);
    // F_out.open(sout,std::ios::out);
    // F_in>>s;
    // F_out<<s;
    return 0;
}