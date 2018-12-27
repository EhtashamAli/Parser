#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
class QUADRUPLE {
    private:
      
    public:
        string op;
        string arg1;
        string arg2;
        string target;
        QUADRUPLE(string op , string arg1 , string arg2 , string target){
            this->op = op;
            this->arg1 = arg1;
            this->arg2 = arg2;
            this->target = target;
        }
        bool setOP(string op){
            this->op = op;
            return true;
        }
        bool setArg1(string arg1){
            this->arg1;
            return true;
        }
        bool setArg2(string arg2){
            this->arg2;
            return true;
        }
        bool setTarget(string target){
            this->target;
            return true;
        }
        
};

#endif
