#include "lexical_analysis.h"

#include <string.h>
#include <ctype.h>
#include <assert.h>


// static ---------------------------------------------------------------------


static bool match(Lexer* lexer, char expected);
static Token errorToken(const char* message);
static Token readIdentifier(Lexer* lexer);
static Token readString(Lexer* lexer);
static Token readNumber(Lexer* lexer);
static Token makeToken(Lexer* lexer, TokenType type);


// public --------------------------------------------------------------------


void initLexer(Lexer* lexer, const char* source)
{
    assert(lexer  != NULL);
    assert(source != NULL);

    lexer->start   = source;
    lexer->current = source;
    lexer->line    = 1;

    return;
}


Token nextToken(Lexer* lexer)
{
    assert(lexer != NULL);

    while (true)
    {
        lexer->start = lexer->current;
        if (*lexer->current == '\0')
        {
            return makeToken(lexer, TOKEN_EOF); 
        } 

        char current_char = *lexer->current++;
        if (current_char == ' '
         || current_char == '\t'
         || current_char == '\r')
        {
            continue;
        }

        if (current_char == '/' && *lexer->current  == '/')
        {
            lexer->current++;
            continue;
        }

        switch (current_char)
        {
            case '\n': lexer->line++; break;
            case '(' : return makeToken(lexer, TOKEN_LPAREN);
            case ')' : return makeToken(lexer, TOKEN_RPAREN);
            case '{' : return makeToken(lexer, TOKEN_LBRACE);
            case '}' : return makeToken(lexer, TOKEN_RBRACE);
            case ';' : return makeToken(lexer, TOKEN_SEMICOLON);
            case '+' : return makeToken(lexer, TOKEN_PLUS);
            case '-' : return makeToken(lexer, TOKEN_MINUS);
            case '*' : return makeToken(lexer, TOKEN_STAR);
            case '/' : return makeToken(lexer, TOKEN_SLASH);
            case '%' : return makeToken(lexer, TOKEN_PERCENT);

            case '=':
                if (match(lexer, '=')) 
                {
                    return makeToken(lexer, TOKEN_EQEQ);
                }
                else
                {
                    return makeToken(lexer, TOKEN_EQ);
                }

            case '!':
                if (match(lexer, '='))
                {
                    return makeToken(lexer, TOKEN_BANGEQ);
                }
                else
                {
                    return makeToken(lexer, TOKEN_BANG);
                }

            case '<':
                if (match(lexer, '='))
                {
                    return makeToken(lexer, TOKEN_LTEQ);
                }
                else
                {
                    return makeToken(lexer, TOKEN_LT);
                }  
            
            case '>':
                if (match(lexer, '='))
                {
                    return makeToken(lexer, TOKEN_GTEQ);
                }
                else
                {
                    return makeToken(lexer, TOKEN_GT);
                }  

            case '"': return readString(lexer);
            
            default:
                if (isdigit(current_char))
                {
                    return readNumber(lexer);
                }
                if (isalpha(current_char))
                {
                    return readIdentifier(lexer); 
                }
                return errorToken("Unexpected character");
        }
    }
}


// static ---------------------------------------------------------------------


static Token makeToken(Lexer* lexer, TokenType type)
{
    assert(lexer != NULL);
    
    return (Token){
        .type   = type,
        .start  = lexer->start,
        .length = (int)(lexer->current - lexer->start),
        .line   = lexer->line,
    };    
}


static Token readNumber(Lexer* lexer)
{
    assert(lexer != NULL);

    while (isdigit(*lexer->current))
    {
        lexer->current++;
    }

    if (*lexer->current == '.' && isdigit(lexer->current[1]))
    {
        lexer->current++;
        while (isdigit(*lexer->current))
        {
            lexer->current++;
        }
    }

    return makeToken(lexer, TOKEN_NUMBER);
}


static Token readString(Lexer* lexer)
{
    assert(lexer != NULL);
    
    while (*lexer->current != '"' && *lexer->current != '\0')
    {
        if (*lexer->current == '\n')
        {
            lexer->line++;
            lexer->current++;
        }
    }

    if (*lexer->current == '\0')
    {
        return errorToken("Unterminated string");
    }

    lexer->current++;
    return makeToken(lexer, TOKEN_STRING);
}


static Token readIdentifier(Lexer* lexer)
{
    assert(lexer != NULL);

    while (isalpha(*lexer->current)
        || isdigit(*lexer->current)
        || *lexer->current == '_')
    {
        lexer->current++; 
    }

    int identifier_length = (int)(lexer->current - lexer->start);
    for (size_t i = 0; i < KEYWORD_ARRAY_LENGTH; i++)
    {
        const char* keyword = KEYWORD_ARRAY[i].keyword;
        int keyword_length = (int)strlen(keyword);

        if (identifier_length == keyword_length
         && !memcmp(lexer->start, keyword, (size_t)keyword_length))
        {
            return makeToken(lexer, KEYWORD_ARRAY[i].type); 
        }
    }

    return makeToken(lexer, TOKEN_IDENTIFIER);
}


static Token errorToken(const char* message)
{
    return (Token){
        .type   = TOKEN_ERROR,
        .start  = message,
        .length = (int)strlen(message),
        .line   = 0,
    };
}


static bool match(Lexer* lexer, char expected)
{
    assert(lexer != NULL);

    if (*lexer->current == '\0')
    {
        return false;
    }
    if (*lexer->current != expected)
    {
        return false; 
    }

    lexer->current++;
    return true;
}
