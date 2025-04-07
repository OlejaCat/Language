#ifndef SYNTACTIC_ANALYSIS
#define SYNTACTIC_ANALYSIS

#include "syntactic_analysis_struct.h"
#include "lexical_analysis.h"
#include "tree.h"

typedef struct Parser
{
    Lexer* lexer;
    Token  current_token;
    Tree*  ast;
    int    current_node;
} Parser;

void initParser(Parser* parser, Lexer* lexer);
void dtorParser(Parser* parser);
void parseProgram(Parser* parser);

#endif
