#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../TOKEN/token.h"
#include "../STORE/quadruple.cpp"

using namespace std;

class PARSER {
    private:
        // STORE s;
        int CURSOR = 0;
        int SAVED_CURSOR = 0;
        int POINTER = 0;
        vector<TOKEN> TOKENS;
        ////////////////////////////////
        map<string, string> declarations;
        map<string, string>::iterator IT;
        pair<map<string, string>::iterator,bool> ret;
        bool ifExist(string) ;
        bool insert(string,string);
        void print();
        void printQuad();
        string ifType(string);
        vector <QUADRUPLE> QUADRUPLES;
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
        /////////////////////////
        bool INPUT_STATEMENTS();
        bool OUTPUT_STATEMENTS();
        bool OUTPUTS();
    public:
        void PARSE(vector<TOKEN>);
};

#endif