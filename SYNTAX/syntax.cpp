#include "./syntax.h"
#include "../KEYWORDS/KEYWORDS.h"
#include "../STORE/store.cpp"
#include <sstream> 
int lineCount = 0;

void PARSER::PARSE(vector<TOKEN> tokens){
    this->CURSOR = 0;
    this->SAVED_CURSOR = 0;
    this->TOKENS = tokens;
    if(this->MAIN() && CURSOR == this->TOKENS.size()){
        s.print();
        cout<<"PARSING SUCCESSFUL"<<endl;
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
        // cout << next.lexeme << "||" << next.type<< endl;
        return next;
    } else {
        // cout << next.lexeme << "||" << next.type << endl;
        return next;
    }
}
TOKEN PARSER::peekToken(){
    int pointer = CURSOR;
    TOKEN next = this->TOKENS[pointer];
    pointer++;
    if(next.type == "NEWLINE"){
        next = this->TOKENS[pointer];
        pointer++;
    } else {
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
   return   (save_cursor() && FUNCTION_STATEMENT() && MAIN()) ||
            (back_track() && save_cursor() && DECLARATION_STATEMENT() && MAIN()) ||
            (back_track() && save_cursor() && TERM_TYPE(DATATYPE) && TERM(KW_CONTAINER) && TERM("(")  && TERM(")") && TERM("{") && STATEMENT() && TERM("}")) ||
            (back_track() && true);
}
bool PARSER::STATEMENT(){
    return (save_cursor() && DECLARATION_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && ASSIGNMENT_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && FUNCTION_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && INC_DEC_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && IF_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && ITERATION_STATEMENT() && STATEMENT()) ||
           (back_track() && save_cursor() && INPUT_STATEMENTS() && STATEMENT()) ||
           (back_track() && save_cursor() && OUTPUT_STATEMENTS() && STATEMENT()) ||
           (back_track() && true);
}
bool PARSER::INPUT_STATEMENTS(){
    return (TERM_TYPE(INPUT) && TERM(":") && TERM_TYPE(IDENTIFIER));
}

bool PARSER::OUTPUT_STATEMENTS(){
    return (TERM_TYPE(OUTPUT) && OUTPUTS());
}
bool PARSER::OUTPUTS(){
            return (save_cursor() && TERM(":") && TERM_TYPE(IDENTIFIER) && OUTPUTS()) ||
            (back_track() && save_cursor() && TERM(":") && TERM_TYPE(STRING) && OUTPUTS()) ||
            (back_track() && true);
}
bool PARSER::FUNCTION_STATEMENT(){
      return (TERM_TYPE(DATATYPE) && TERM_TYPE(FUNCTION) && TERM_TYPE(IDENTIFIER) && TERM("(") && PARAMETERS() && TERM(")") && TERM("{") && STATEMENT() && TERM("}"));
}

bool PARSER::PARAMETERS(){
    return  (save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER) && PARAMETERS()) ||
            (back_track() && save_cursor() && TERM(",") && PARAMETERS()) ||
            (back_track() && true);
}
bool PARSER::DECLARATION_STATEMENT(){
    // return  (save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && EXPRESSION()) ||
    //         (back_track() && save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && TERM_TYPE(IDENTIFIER));
    if(save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER) && TERM("=")){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        string key = getNextToken().lexeme;
        if(s.ifExist(key)){
            cout<<"symantic error:  "<<key<<"  already declared"<<endl;
        }else{
           s.insert(key,var);;
        }
        getNextToken();
        if(EXPRESSION()){
            return true;
        }
           return false;
    }
    //    return false;
    //     // cout<<getNextToken().lexeme<<getNextToken().lexeme<<getNextToken().lexeme<<EXPRESSION()<<endl;
    // }
    if(back_track() && save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER) && TERM("=") && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        string key = getNextToken().lexeme;
        if(s.ifExist(key)){
            cout<<"symantic error:  "<<key<<"  already declared"<<endl;
        }else{
            s.insert(key,var);
        }
        getNextToken();
        string toCheck = getNextToken().lexeme;
        if(s.ifExist(toCheck)){
            cout<<"exist1  "<<toCheck<<endl;
            return true; 
        }else{
            cout<<"SEMANTIC ERROR :"<<toCheck<<" not declared"<<endl;
            return true;
        }
        return true;
    }   
    return false;  
}
bool PARSER::ASSIGNMENT_STATEMENT(){
    // return (TERM_TYPE(IDENTIFIER) && TERM("=") && EXPRESSION());
    if(save_cursor() && TERM_TYPE(IDENTIFIER) && TERM("=")){
        back_track();save_cursor();
        string key = getNextToken().lexeme;
        if(s.ifExist(key)){
            cout<<"exist2  "<<key<<endl; 
        }else{
            cout<<"SEMANTIC ERROR :"<<key<<" not declared"<<endl;
        }
        getNextToken();
        if(EXPRESSION()){
            return true;
        }
           return false;
    }
       return false;
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
    // return  (save_cursor() && TERM("(") && EXPRESSION() && TERM(")")) ||
    //         (back_track() && save_cursor() && TERM_TYPE(IDENTIFIER)) ||
    //         (back_track() && save_cursor() && TERM_TYPE(NUMBER));
    if(save_cursor() && TERM("(") && EXPRESSION() && TERM(")")){
        return true;
    }
    if(back_track() && save_cursor() && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        if(s.ifExist(var)){
            cout<<"exist  "<<var<<endl;
            return true; 
        }else{
            cout<<"SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            return true;
        }
    }
    if(back_track() && save_cursor() && TERM_TYPE(NUMBER)){
        return true;
    }
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
    return
            (save_cursor() && TERM_TYPE(IDENTIFIER)) ||
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
    return  (save_cursor() && LOOP()) ||
            (back_track() && save_cursor() && WHILE()) ||
            (back_track() && save_cursor() && DO_WHILE());
}
bool PARSER::LOOP(){
    return  (TERM("loop") && TERM("(") && DECLARATION_STATEMENT() && TERM(";") && CONDITION() && TERM(";") && INC_DEC_STATEMENT() && TERM(")") && TERM("{") && STATEMENT() && TERM("}"));
}
bool PARSER::WHILE(){
    return  (TERM("loopuntil") && TERM("(") && CONDITION() && TERM(")") && TERM("{") && STATEMENT() && TERM("}"));
}
bool PARSER::DO_WHILE(){
    return  (TERM("loop") && TERM("{") && STATEMENT() && TERM("}") && TERM("until") && TERM("(") && CONDITION() && TERM(")"));
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
