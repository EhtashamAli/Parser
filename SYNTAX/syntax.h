#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../TOKEN/token.h"
#include "../STORE/quadruple.cpp"
#include "../STORE/temps.cpp"

using namespace std;

class PARSER {
    private:
        // STORE s;
        int CURSOR = 0;
        int SAVED_CURSOR = 0;
        int POINTER = 0;
        int LABEL = 0;
        int TARGET = 0;
        int TEMP = 0;
        ////////////////////////////////
        vector <QUADRUPLE> QUADRUPLES;
        vector<TOKEN> TOKENS;
        vector<string> LABELS;
        vector<TEMPS> V_TEMP;
        ////////////////////////////////
        map<string, string> declarations;
        map<string, string>::iterator IT;
        pair<map<string, string>::iterator,bool> ret;
        /////////////////////////////////
        string getTemp(string key){
            for(int i = 0 ; i < this->V_TEMP.size() ; i++){
                if(V_TEMP[i].id == key){
                    return V_TEMP[i].temp;
                }
            }
        }
        string genLabel(){
            string val = "_LABEL"+to_string(this->LABEL);
            this->LABEL++;
            return val;
        }
        string genTarget(){
            string val = "_T"+to_string(this->TARGET);
            this->TARGET++;
            return val;
        }
        string genTemp(){
            string val = "_t"+to_string(this->TEMP);
            this->TEMP++;
            return val;
        }
        bool ifExist(string) ;
        bool insert(string,string);
        void print();
        void printQuad();
        string ifType(string);
        ///////////////////////////////
        string LAST_EXPECTED = "";
        string LAST_EXPECTED_TYPE = "";
        bool save_cursor();
        bool back_track();
        TOKEN getNextToken();
        bool TERM(string);
        bool TERM_TYPE(string);
        bool MAIN();
        bool STATEMENT();
        bool ASSIGNMENT_STATEMENT();
        bool DECLARATION_STATEMENT();
        bool EXP_DECLARATION_STATEMENT();
        bool INC_DEC_STATEMENT();
        TOKEN peekToken();
        //EXPRESSION
        bool EXPRESSION();
        bool TERM();
        bool EXPRESSION1();
        bool TERM1();
        bool FACTOR();
        /////////////
        // inc_dec //
        bool pre_inc();
        bool post_inc();
        bool pre_dec();
        bool post_dec();
        ////////////////
        //if_statemnt //
        bool IF_STATEMENT();
        bool MATCHED();
        bool OPTIONAL_TAIL();
        bool TAIL();
        ////////////////
        // CONDITION  //
        bool CONDITION();
        bool LT_OP();
        bool GT_OP();
        bool EQ_OP();
        bool NE_OP();
        bool GE_OP();
        bool LE_OP();
        bool TYPE();
        /////////////////////////
        // ITERATION_STATEMENT //
        bool ITERATION_STATEMENT();
        bool LOOP();
        bool WHILE();
        bool DO_WHILE();
        /////////////////////////
        bool FUNCTION_STATEMENT();
        bool PARAMETERS();
        bool FUNCTION_CALL();
        bool ARGUMENTS();
        /////////////////////////
        bool INPUT_STATEMENTS();
        bool OUTPUT_STATEMENTS();
        bool OUTPUTS();
    public:
        void PARSE(vector<TOKEN>);
};

#endif