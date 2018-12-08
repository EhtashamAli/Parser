#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <string>
#include <vector>
#include "../TOKEN/token.h"

using namespace std;

class PARSER {
    private:
        int CURSOR = 0;
        int SAVED_CURSOR = 0;
        vector<TOKEN> TOKENS;
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
        bool INC_DEC_STATEMENT();
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
    public:
        void PARSE(vector<TOKEN>);
};

#endif