#include "./syntax.h"
#include "../KEYWORDS/KEYWORDS.h"
#include <sstream> 
int lineCount = 0;


void PARSER::PARSE(vector<TOKEN> tokens){
    this->CURSOR = 0;
    this->SAVED_CURSOR = 0;
    this->TOKENS = tokens;
    if(this->MAIN() && CURSOR == this->TOKENS.size()){
        cout<<"PARSING SUCCESSFUL";
    } else {
        cout<<"PARSING FAILED on line:" <<lineCount+2<<" "<<LAST_EXPECTED<<" "<<LAST_EXPECTED_TYPE<<endl;
    }
}

bool PARSER::save_cursor(){
    this->SAVED_CURSOR = this->CURSOR;
    return true;
}

bool PARSER::back_track(){
    this->CURSOR = this->SAVED_CURSOR;
    return true;
}

TOKEN PARSER::getNextToken(){
    TOKEN next = this->TOKENS[CURSOR];
    this->CURSOR++;
    if(next.type == "NEWLINE"){
        stringstream geek(next.lexeme);
        geek >> lineCount;
        next = this->TOKENS[CURSOR];
        this->CURSOR++;
        cout << next.lexeme << "||" << next.type<< endl;
        return next;
    } else {
        cout << next.lexeme << "||" << next.type << endl;
        return next;
    }
}

bool PARSER::TERM(string expected){
    if(getNextToken().lexeme == expected){
        return true;
    } else {
        LAST_EXPECTED = expected;
        return false;
    }
    // return getNextToken().lexeme == expected;
}

bool PARSER::TERM_TYPE(string expected){
    if(getNextToken().type == expected){
        return true;
    } else {
        LAST_EXPECTED_TYPE = expected;
        return false;
    }
}


bool PARSER::MAIN(){
   return (TERM_TYPE(KEYWORD) && TERM(KW_CONTAINER) && TERM("(")  && TERM(")") && TERM("{") && STATEMENT() && TERM("}")); 
}

bool PARSER::STATEMENT(){
    return (save_cursor() && DECLARATION_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && ASSIGNMENT_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && INC_DEC_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && IF_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && ITERATION_STATEMENT() && STATEMENT()) ||
           (back_track() && true);
}

bool PARSER::DECLARATION_STATEMENT(){
    return  (save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && EXPRESSION()) ||
            (back_track() && save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && TERM_TYPE(IDENTIFIER));
           
}
bool PARSER::ASSIGNMENT_STATEMENT(){
    return (TERM_TYPE(IDENTIFIER) && TERM("=") && EXPRESSION());
}
// E  -> T E'
// E' -> + T E' | -TE' |epsilon
// T  -> F T'
// T' -> * F T' | /FT' |epsilon
// F  -> (E) | int
bool PARSER::EXPRESSION(){
    return (TERM() && EXPRESSION1());
}
bool PARSER::EXPRESSION1(){
    return  (save_cursor() && TERM("+") && TERM() && EXPRESSION1()) ||
            (back_track() && save_cursor() && TERM("-") && TERM() && EXPRESSION1()) ||
            (back_track() && true);
}
bool PARSER::TERM(){
    return (FACTOR() && TERM1());
}
bool PARSER::TERM1(){
    return  (save_cursor() && TERM("*") && FACTOR() && TERM1()) ||
            (back_track() && save_cursor() && TERM("/") && FACTOR() && TERM1()) ||
            (back_track() && true);
}
bool PARSER::FACTOR(){
    return  (save_cursor() && TERM("(") && EXPRESSION() && TERM(")")) ||
            (back_track() && save_cursor() && TERM_TYPE(IDENTIFIER)) ||
            (back_track() && save_cursor() && TERM_TYPE(NUMBER));
}

// inc_dec_Statement -> pre_inc |
//                          post_inc |
//                          pre_dec |
//                          post_dec
bool PARSER::INC_DEC_STATEMENT(){
    return  (save_cursor() && pre_inc()) ||
            (back_track() & save_cursor() && pre_dec()) ||
            (back_track() & save_cursor() && post_dec()) ||
            (back_track() & save_cursor() && post_inc());
}
bool PARSER::pre_inc(){
    return (TERM("+") && TERM("+") && TERM_TYPE(IDENTIFIER));
}
bool PARSER::pre_dec(){
    return (TERM("-") && TERM("-") && TERM_TYPE(IDENTIFIER));
}
bool PARSER::post_dec(){
    return (TERM_TYPE(IDENTIFIER) && TERM("-") && TERM("-"));
}
bool PARSER::post_inc(){
    return (TERM_TYPE(IDENTIFIER) && TERM("+") && TERM("+"));
}

// statement → if (E) matched optional-tail | other
// matched → if (E) matched else matched | other
// optional-tail → else tail | ε
// tail → if (E) tail | other

// IF_Stmt -> if ( Expr_IF ) Stmt Optional_Else_Stmt
// Optional_Else_Stmt -> (empty-string)
//                       |else Stmt.

bool PARSER::IF_STATEMENT(){
    return  (TERM("this") && TERM("(") && CONDITION() && TERM(")") && TERM("{") && STATEMENT() && TERM("}") && OPTIONAL_TAIL());
}

bool PARSER::OPTIONAL_TAIL(){
    return  (save_cursor() && TERM("then") && TERM("{") && STATEMENT() && TERM("}")) ||
            (back_track() && true);
}
////////////////////////////////////

bool PARSER::CONDITION(){
    return (save_cursor() && LE_OP()) ||
           (back_track() && save_cursor() && GE_OP()) ||
           (back_track() && save_cursor() && LT_OP()) ||
           (back_track() && save_cursor() && GT_OP()) ||
           (back_track() && save_cursor() && EQ_OP()) ||
           (back_track() && save_cursor() && NE_OP());
}
bool PARSER::TYPE(){
    return  (save_cursor() && TERM_TYPE(IDENTIFIER)) ||
            (back_track() && save_cursor() && TERM_TYPE(NUMBER));
}
bool PARSER::LT_OP(){
    return (TYPE() && TERM("<") && TYPE());
}
bool PARSER::GT_OP(){
    return (TYPE() && TERM(">") && TYPE());
}
bool PARSER::EQ_OP(){
    return (TYPE() && TERM("=") && TERM("=") && TYPE());
}
bool PARSER::NE_OP(){
    return (TYPE() && TERM("!") && TERM("=") && TYPE());
}
bool PARSER::LE_OP(){
    return (TYPE() && TERM("<") && TERM("=") && TYPE());
}
bool PARSER::GE_OP(){
    return (TYPE() && TERM(">") && TERM("=") && TYPE());
}
// bool PARSER::MATCHED(){
//     return  (TERM("this") && MATCHED() && TERM("then") && MATCHED());
// }
// bool PARSER::TAIL(){
//     return  (TERM("this") && TAIL());
// }
// 

bool PARSER::ITERATION_STATEMENT(){
    return (LOOP());
}
bool PARSER::LOOP(){
    return  (TERM("LOOP") && TERM("(") && DECLARATION_STATEMENT() && TERM(";") && CONDITION() && TERM(";") && INC_DEC_STATEMENT() && TERM(")") && TERM("{") && STATEMENT() && TERM("}"));
}
