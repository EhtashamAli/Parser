#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream> 
#include "../STORE/quadruple.h"
#include "../SYNTAX/syntax.h"
using namespace std;

class CodeGenerator{
    vector<QUADRUPLE> quads;
    ofstream OUTPUT_FILE;
    public:
    CodeGenerator(vector<QUADRUPLE> quad){
        this->quads = quad;
    }
    void generateAssembly(){
        OUTPUT_FILE.open("../assembly.s");
        OUTPUT_FILE<<".data"<<endl;
        OUTPUT_FILE<<".text"<<endl;
        OUTPUT_FILE<<".globl main"<<endl;
        OUTPUT_FILE<<"main:"<<endl;
        string val;
        for (vector<QUADRUPLE>::const_iterator iter = quads.begin();
         iter != quads.end(); ++iter){
                
                    if(iter->op == "+"){
                        OUTPUT_FILE<< "add" << " " << "$"<<iter->target<<" , " << "$"<<iter->arg1<<" , " << "$"<<iter->arg2 <<endl;
                    }
                    else if(iter->op == "-"){
                        OUTPUT_FILE<< "sub" << " " << "$"<< iter->target<<" , "<< "$" <<iter->arg1<<" , "<< "$" <<iter->arg2 <<endl;
                    }
                    else if(iter->op == "*"){
                        OUTPUT_FILE<< "mul" << " " << "$"<< iter->target<<" , " << "$"<<iter->arg1<<" , " << "$"<<iter->arg2 <<endl;
                    }
                    else if(iter->op == "/"){
                        OUTPUT_FILE<< "div" << " " << "$"<< iter->target<<" , " << "$"<<iter->arg1<<" , "<< "$" <<iter->arg2 <<endl;
                    }
                    else if(iter->target == "" || iter->target == "if" ){
                        OUTPUT_FILE<<iter->target <<"" <<  iter->arg1<< " " <<iter->op << " " << iter->arg2<< " " <<  endl;
                    }
                    else{
                        OUTPUT_FILE<< "li" << " " << "$"<< iter->target<<" , "<< "$" <<iter->arg1 <<endl; 
                    }
                
                
         }
    }
};

