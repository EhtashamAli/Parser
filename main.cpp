#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./LEXER/lexer.h"
#include "./LEXER/lexer.cpp"
#include "./TOKEN/token.h"
#include "./SYNTAX/syntax.h"
#include "./STORE/quadruple.h"
#include "./SYNTAX/syntax.cpp"
#include "./Backend/codeGenerator.cpp"

using namespace std;

int main(){
    vector<TOKEN> tokens;
    vector<QUADRUPLE> quad;
    LEXER lex("asda");
    lex.generateTokens();
    tokens = lex.getTokens();
    cout<<tokens.size()<<endl;
    PARSER parser;
    parser.PARSE(tokens);
    quad = parser.getQuadruples();
    CodeGenerator code(quad);
    code.generateAssembly(); 
    return 0;
}