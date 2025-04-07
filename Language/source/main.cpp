#include <stdio.h>

#include "lexical_analysis.h"
#include "syntactic_analysis.h"
#include "print_ast.h"


int main()
{
    const char* source = "S = 1231 + 10 * 2; if (S > 10) {S = 5;}";
    Lexer lexer = {}; 
    initLexer(&lexer, source);

    Parser parser = {};
    initParser(&parser, &lexer);

    parseProgram(&parser);
    printASTFromRoot(parser.ast);

    dtorParser(&parser);

    return 0;
}
