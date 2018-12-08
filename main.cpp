#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./LEXER/lexer.h"
#include "./LEXER/lexer.cpp"
#include "./TOKEN/token.h"
#include "./SYNTAX/syntax.h"
#include "./SYNTAX/syntax.cpp"
using namespace std;

int main(){
    vector<TOKEN> tokens;
    LEXER lex("asda");
    lex.generateTokens();
    tokens = lex.getTokens();
    cout<<tokens.size()<<endl;
    PARSER parser;
    parser.PARSE(tokens);
    return 0;
}