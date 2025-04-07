#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum TokenType
{
    TOKEN_EOF           = 0,
    TOKEN_NUMBER        = 1,
    TOKEN_STRING        = 2,
    TOKEN_IDENTIFIER    = 3,
    TOKEN_KEYWORD_VAR   = 4,
    TOKEN_KEYWORD_IF    = 5,
    TOKEN_KEYWORD_ELSE  = 6,
    TOKEN_KEYWORD_WHILE = 7,
    TOKEN_PRINT         = 8,
    TOKEN_PLUS          = 9,
    TOKEN_MINUS         = 10,
    TOKEN_STAR          = 11,
    TOKEN_SLASH         = 12,
    TOKEN_PERCENT       = 13,
    TOKEN_BANG          = 14,
    TOKEN_BANGEQ        = 15,
    TOKEN_EQ            = 16,
    TOKEN_EQEQ          = 17,
    TOKEN_LT            = 18,
    TOKEN_GT            = 19,
    TOKEN_LTEQ          = 20,
    TOKEN_GTEQ          = 21,
    TOKEN_AND           = 22,
    TOKEN_OR            = 23,
    TOKEN_LPAREN        = 24,
    TOKEN_RPAREN        = 25,
    TOKEN_LBRACE        = 26,
    TOKEN_RBRACE        = 27,
    TOKEN_SEMICOLON     = 28,
    TOKEN_ERROR         = 29,
} TokenType;

typedef struct Token
{
    TokenType   type;
    const char* start;
    int         length;
    int         line;
} Token;

typedef struct Keyword
{
    const char* keyword;
    TokenType   type;
} Keyword;

typedef struct Lexer
{
    const char* start;
    const char* current;
    int         line;
} Lexer;

const Keyword KEYWORD_ARRAY[] = {
    {.keyword = "var"  , .type = TOKEN_KEYWORD_VAR  },
    {.keyword = "if"   , .type = TOKEN_KEYWORD_IF   },
    {.keyword = "else" , .type = TOKEN_KEYWORD_ELSE },
    {.keyword = "while", .type = TOKEN_KEYWORD_WHILE},
    {.keyword = "print", .type = TOKEN_PRINT        },
};

const size_t KEYWORD_ARRAY_LENGTH = sizeof(KEYWORD_ARRAY) / sizeof(Keyword);

void initLexer(Lexer* lexer, const char* source);
void dtorLexer(Lexer* lexer);
Token nextToken(Lexer* lexer);

#endif

