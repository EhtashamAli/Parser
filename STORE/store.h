#ifndef STORE_H
#define STORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map> 
#include "./quadruple.cpp"
using namespace std;
class STORE {
    private:
        map<string, string> declarations;
        map<string, string>::iterator IT;
        pair<map<string, string>::iterator,bool> ret;
    public : 
        bool ifExist(string) ;
        bool insert(string,string);
        void print();
        string ifType(string);
        int POINTER = 0;
        vector <QUADRUPLE> QUADRUPLES;
};




#endif