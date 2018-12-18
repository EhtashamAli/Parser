#include "./syntax.h"
#include "../KEYWORDS/KEYWORDS.h"
// #include "../STORE/store.cpp"
#include <sstream> 
int lineCount = 0;

////////////////////////SEMANTIC//////////////////////////////
bool PARSER::ifExist(string el) {
    // cout<<this->declarations.find(el)->second<<endl;
    this->IT = this->declarations.find(el);
    // cout<<IT->second<<endl;
    if(this->IT == this->declarations.end()){
        return false;
    } else {
        return true;
    }
}
bool PARSER::insert(string key , string val){
    this->ret = this->declarations.insert (pair<string,string>(key,val));
    if (ret.second==false) {
        cout << "element " << key <<" already existed"<<endl;
        return false;
    } else {
        return true;
    }  
}
void PARSER::print(){
     for (IT=declarations.begin(); IT!=declarations.end(); ++IT)
    std::cout <<"| "<<IT->first <<" | "<< " => " <<"| " <<IT->second <<" |"<< '\n';
}

void PARSER::printQuad(){
    int iter;
    for (vector<QUADRUPLE>::const_iterator iter = QUADRUPLES.begin();
         iter != QUADRUPLES.end(); ++iter)
            std::cout << iter->op << "\t" << iter->arg1<< "\t" << iter->arg2<< "\t" << iter->target<< "\t" << endl;
}
string PARSER::ifType(string el){
    this->IT = this->declarations.find(el);
    if(this->IT == this->declarations.end()){
        return "";
    } else {
        return this->IT->second;
    }
}
/////////////////////////////////////////////////////////////////////////////
void PARSER::PARSE(vector<TOKEN> tokens){
    this->CURSOR = 0;
    this->SAVED_CURSOR = 0;
    this->TOKENS = tokens;
    if(this->MAIN() && CURSOR == this->TOKENS.size()){
        this->print();
        this->printQuad();
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
    return  (save_cursor() && FUNCTION_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && DECLARATION_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && FUNCTION_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && ASSIGNMENT_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && INC_DEC_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && IF_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && ITERATION_STATEMENT() && STATEMENT()) ||
            (back_track() && save_cursor() && INPUT_STATEMENTS() && STATEMENT()) ||
            (back_track() && save_cursor() && OUTPUT_STATEMENTS() && STATEMENT()) ||
            (back_track() && true);
}
bool PARSER::INPUT_STATEMENTS(){
    // return (TERM_TYPE(INPUT) && TERM(":") && TERM_TYPE(IDENTIFIER));
    if(TERM_TYPE(INPUT) && TERM(":") && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        getNextToken();
        getNextToken();
        string toCheck = getNextToken().lexeme;
        if(this->ifExist(toCheck)){
            // cout<<"exist1  "<<toCheck<<endl;
        }else{
            cout<<"\t SEMANTIC ERROR :"<<toCheck<<" not declared"<<endl;
        }
        return true;
    }
    return false;
}

bool PARSER::OUTPUT_STATEMENTS(){
    return (TERM_TYPE(OUTPUT) && OUTPUTS());
}
bool PARSER::OUTPUTS(){
            // return (save_cursor() && TERM(":") && TERM_TYPE(IDENTIFIER) && OUTPUTS()) ||
            // (back_track() && save_cursor() && TERM(":") && TERM_TYPE(STRING) && OUTPUTS()) ||
            // (back_track() && true);
            if(save_cursor() && TERM(":") && TERM_TYPE(IDENTIFIER)){
                back_track();save_cursor();
                getNextToken();
                string toCheck = getNextToken().lexeme;
                // cout<<s.ifType(toCheck)<<endl;
                if(this->ifExist(toCheck)){
                    // cout<<"exist1  "<<toCheck<<endl;
                }else{
                    cout<<"\t SEMANTIC ERROR :"<<toCheck<<" not declared"<<endl;
                }
                OUTPUTS();
                return true;
            }
            if(back_track() && save_cursor() && TERM(":") && TERM_TYPE(STRING) && OUTPUTS()){
                return true;
            }
            if(back_track() && true){
                return true;
            }
            return false;
}
bool PARSER::FUNCTION_STATEMENT(){
    //   return (TERM_TYPE(DATATYPE) && TERM_TYPE(FUNCTION) && TERM_TYPE(IDENTIFIER) && TERM("(") && PARAMETERS() && TERM(")") && TERM("{") && STATEMENT() && TERM("}"));
    if(TERM_TYPE(DATATYPE) && TERM_TYPE(FUNCTION) && TERM_TYPE(IDENTIFIER)){
        back_track() && save_cursor();
        getNextToken();getNextToken();
        string key = getNextToken().lexeme;
        if(this->ifExist(key)){
            cout<<"\t SEMANTIC error:  "<<key<<"  already declared"<<endl;
        }else{
           this->insert(key,"function");
        }
        if(TERM("(") && PARAMETERS() && TERM(")") && TERM("{") && STATEMENT() && TERM("}")){
            return true;
        }
    }
    return false;
}

bool PARSER::PARAMETERS(){
    // return  (save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER) && PARAMETERS()) ||
    //         (back_track() && save_cursor() && TERM(",") && PARAMETERS()) ||
    //         (back_track() && true);
    if(save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        string key = getNextToken().lexeme;
        // cout<<var<<endl;
        if(this->ifExist(key)){
            cout<<"\t SEMANTIC error:  "<<key<<"  already declared"<<endl;
        }else{
           this->insert(key,var);;
        }
        PARAMETERS();
        return true;
    }
    if(back_track() && save_cursor() && TERM(",") && PARAMETERS()){
        return true;
    }
    if(back_track() && true){
        return true;
    }
    return false;
}
bool PARSER::DECLARATION_STATEMENT(){
    // return  (save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && EXPRESSION()) ||
    //         (back_track() && save_cursor() && TERM_TYPE(KEYWORD) && TERM_TYPE(IDENTIFIER) && TERM("=") && TERM_TYPE(IDENTIFIER));
    if(save_cursor() && TERM_TYPE(DATATYPE) && TERM_TYPE(IDENTIFIER) && TERM("=")){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        string key = getNextToken().lexeme;
        if(this->ifExist(key)){
            cout<<"\t SEMANTIC error:  "<<key<<"  already declared"<<endl;
        }else{
           this->insert(key,var);;
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
        if(this->ifExist(key)){
            cout<<"symantic error:  "<<key<<"  already declared"<<endl;
        }else{
            this->insert(key,var);
        }
        getNextToken();
        string toCheck = getNextToken().lexeme;
        if(this->ifExist(toCheck)){
            // cout<<"exist1  "<<toCheck<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<toCheck<<" not declared"<<endl;
            // return true;
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
        if(this->ifExist(key)){
            // cout<<"exist2  "<<key<<endl; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<key<<" not declared"<<endl;
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
    // return  (save_cursor() && TERM("+") && TERM() && EXPRESSION1()) ||
    //         (back_track() && save_cursor() && TERM("-") && TERM() && EXPRESSION1()) ||
    //         (back_track() && true);
    if(save_cursor() && TERM("+")){
        string op = "";
        string ar1 = "";
        string ar2 = "";
        string tar = "";
        // back_track(); save_cursor();
        // op = getNextToken().lexeme;
        // cout << op << endl;
        // ar1 = getNextToken().lexeme;
        // QUADRUPLE Q =  QUADRUPLE(op , ar1 , "NA" , "TARGETPLUS");
        // this->QUADRUPLES.push_back(Q);
        // back_track(); save_cursor();
        // getNextToken();
         back_track(); save_cursor();
        string last = "";
        // back_track(); save_cursor();
        // string arg1 = getNextToken().lexeme;
        op = getNextToken().lexeme;
        cout << op << endl;
        string arg2 = getNextToken().lexeme;
        if (!QUADRUPLES.empty())
                 last = QUADRUPLES.back().target;
        QUADRUPLE Q =  QUADRUPLE(op , arg2  , last , "TARGETplus");
        this->QUADRUPLES.push_back(Q);
        back_track(); save_cursor();
        getNextToken();
        if(TERM() && EXPRESSION1()){
            return true; 
        }
        return false;
    }
    if(back_track() && save_cursor() && TERM("-")){
        string op = "";
        string ar1 = "";
        string ar2 = "";
        string tar = "";
        // back_track(); save_cursor();
        // op = getNextToken().lexeme;
        // cout << op << endl;
        // string ar1 = getNextToken().lexeme;
        // QUADRUPLE Q =  QUADRUPLE(op , ar1 , "NA" , "TARGETMINUS");
        // this->QUADRUPLES.push_back(Q);
        // back_track(); save_cursor();
        // getNextToken();
         back_track(); save_cursor();
        string last = "";
        // back_track(); save_cursor();
        // string arg1 = getNextToken().lexeme;
        op = getNextToken().lexeme;
        cout << op << endl;
        string arg2 = getNextToken().lexeme;
        if (!QUADRUPLES.empty())
                 last = QUADRUPLES.back().target;
        QUADRUPLE Q =  QUADRUPLE(op , last  , arg2 , "TARGETMinus");
        this->QUADRUPLES.push_back(Q);
        back_track(); save_cursor();
        getNextToken();
        if(TERM() && EXPRESSION1()){
            return true;
        }
        return false;
    }
    if(back_track() && true){
        return true;
    }
    return false;
}
bool PARSER::TERM(){
    return (FACTOR() && TERM1());
}
bool PARSER::TERM1(){
    // return  (save_cursor() && TERM("*") && FACTOR() && TERM1()) ||
    //         (back_track() && save_cursor() && TERM("/") && FACTOR() && TERM1()) ||
    //         (back_track() && true);
    if(save_cursor() && TERM("*")){
        string op = "";
        string ar1 = "";
        string ar2 = "";
        string tar = "";
        back_track(); save_cursor();
        string last = "";
        // back_track(); save_cursor();
        // string arg1 = getNextToken().lexeme;
        op = getNextToken().lexeme;
        cout << op << endl;
        string arg2 = getNextToken().lexeme;
        if (!QUADRUPLES.empty())
                 last = QUADRUPLES.back().target;
        QUADRUPLE Q =  QUADRUPLE(op , arg2  , last , "TARGETMul");
        this->QUADRUPLES.push_back(Q);
        back_track(); save_cursor();
        getNextToken();
        if(FACTOR() && TERM1()){
            return true;
        }
        return false;
    }
    if(back_track() && save_cursor() && TERM("/")){
        back_track(); save_cursor();
        string op = "";
        string ar1 = "";
        string ar2 = "";
        string tar = "";
        string last = "";
        // back_track(); save_cursor();
        // string arg1 = getNextToken().lexeme;
        op = getNextToken().lexeme;
        cout << op << endl;
        string arg2 = getNextToken().lexeme;
        if (!QUADRUPLES.empty())
                 last = QUADRUPLES.back().target;
        QUADRUPLE Q =  QUADRUPLE(op , last  , arg2 , "TARGETdiv");
        this->QUADRUPLES.push_back(Q);
        back_track(); save_cursor();
        getNextToken();
        if(FACTOR() && TERM1()){
            return true;
        }
        return false;
    }
    if(back_track() && true){

        return true;
    }
    return false;
}
bool PARSER::FACTOR(){
    // return  (save_cursor() && TERM("(") && EXPRESSION() && TERM(")")) ||
    //         (back_track() && save_cursor() && TERM_TYPE(IDENTIFIER)) ||
    //         (back_track() && save_cursor() && TERM_TYPE(NUMBER));
    if(save_cursor() && TERM("(") && EXPRESSION() && TERM(")")){
        return true;
    }
    if(back_track() && save_cursor() && TERM_TYPE(IDENTIFIER)){
        string op = "";
        string ar1 = "";
        string ar2 = "";
        string tar = "";
        string last = "";
        string LASToP = "";
        back_track();save_cursor();
        string var = getNextToken().lexeme;


        op = getNextToken().lexeme;
        back_track();save_cursor();
        getNextToken();
        if(op == "+" || op == "-" || op == "/" || op == "*"){
            if (!QUADRUPLES.empty())
                 last = QUADRUPLES.back().target;
            QUADRUPLE Q =  QUADRUPLE("NA" , var , last , "OP"+op);
            this->QUADRUPLES.push_back(Q);   
        }


        // if(op == "}" && op != "*" && op != "/"){
        //     if (!QUADRUPLES.empty())
        //          last = QUADRUPLES.back().target;
        //          LASToP =  QUADRUPLES.back().op;
        //     QUADRUPLE Q =  QUADRUPLE(LASToP , var , last , "OP");
        //     this->QUADRUPLES.push_back(Q);   
        // }
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            // return true;
        }
        return true;
    }
    if(back_track() && save_cursor() && TERM_TYPE(NUMBER)){
        return true;
}
    // return false;
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
    // return (TERM("+") && TERM("+") && TERM_TYPE(IDENTIFIER));
    if(TERM("+") && TERM("+") && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        getNextToken();
        getNextToken();
        string var = getNextToken().lexeme;
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            // return true;
        }
        return true;
    }
    return false;
}
bool PARSER::pre_dec(){
    // return (TERM("-") && TERM("-") && TERM_TYPE(IDENTIFIER));
    if(TERM("-") && TERM("-") && TERM_TYPE(IDENTIFIER)){
        back_track();save_cursor();
        getNextToken();
        getNextToken();
        string var = getNextToken().lexeme;
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            // return true;
        }
        return true;
    }
    return false;
}
bool PARSER::post_dec(){
    // return (TERM_TYPE(IDENTIFIER) && TERM("-") && TERM("-"));
    if(TERM_TYPE(IDENTIFIER) && TERM("-") && TERM("-")){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            // return true;
        }
        getNextToken();
        getNextToken();
        return true;
    }
    return false;
}
bool PARSER::post_inc(){
    // return (TERM_TYPE(IDENTIFIER) && TERM("+") && TERM("+"));
    if(TERM_TYPE(IDENTIFIER) && TERM("+") && TERM("+")){
        back_track();save_cursor();
        string var = getNextToken().lexeme;
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<var<<" not declared"<<endl;
            // return true;
        }
        getNextToken();
        getNextToken();
        return true;
    }
    return false;
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
    // return
    //         (save_cursor() && TERM_TYPE(IDENTIFIER)) ||
    //         (back_track() && save_cursor() && TERM_TYPE(NUMBER));
    if(save_cursor() && TERM_TYPE(IDENTIFIER)){
        back_track(); save_cursor();
        string var = getNextToken().lexeme;
        if(this->ifExist(var)){
            // cout<<"exist  "<<var<<endl;
            // return true; 
        }else{
            cout<<"\t SEMANTIC ERROR :"<<var<<" not declared"<<" under line no: "<<lineCount<<endl;
            // return true;
        }
        return true;
    }
    if(back_track() && save_cursor() && TERM_TYPE(NUMBER)){
        return true;
    }
    return false;
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
