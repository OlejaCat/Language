#include "syntactic_analysis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"


// static -----------------------------------------------------------


static int parseStatement(Parser* parser);
static int parseVarDeclaration(Parser* parser);
static int parseAssignment(Parser* parser);
static int parseIf(Parser* parser);
static int parseWhile(Parser* parser);
static int parseBlock(Parser* parser);
static int parseExpression(Parser* parser);
static int parseLogicalOr(Parser* parser);
static int parseLogicalAnd(Parser* parser);
static int parseEquality(Parser* parser);
static int parseRelationalExpression(Parser* parser);
static int parseAdditiveExpression(Parser* parser);
static int parseMultiplicativeExpression(Parser* parser);
static int parseUnaryExpression(Parser* parser);
static int parsePrimaryExpression(Parser* parser);

// static int parsePrint(Parser* parser);

static int createNumberNode(Tree* ast, double value);
static int createIdentifierNode(Tree* ast, const char* name);

static void advance(Parser* parser);
static bool check(Parser* parser, TokenType type);
static bool expect(Parser* parser, TokenType type, 
                   const char* error_message);


// public -----------------------------------------------------------


void initParser(Parser* parser, Lexer* lexer)
{
    assert(parser != NULL);
    assert(lexer  != NULL);

    parser->lexer         = lexer;
    parser->current_token = nextToken(lexer);
    parser->ast           = treeCtor();
}


void parseProgram(Parser* parser)
{
    assert(parser != NULL);

    tree_node_type root_data = {.type = SyntaxNodeType_PROGRAM};
    int root_index = treeCreateNewNode(parser->ast, root_data);
    parser->current_node = root_index;

    while (!check(parser, TOKEN_EOF))
    {
        int statement_node = parseStatement(parser); 
        treeInsertOnLeft(parser->ast, parser->current_node, statement_node);
    }

    return;
}


void dtorParser(Parser* parser)
{
    if (!parser) 
    {
        fprintf(stderr, "No parser\n");
        return;
    }

    treeDtor(parser->ast);
}


// static -----------------------------------------------------------


static int parseStatement(Parser* parser)
{
    assert(parser != NULL);

    if (check(parser, TOKEN_KEYWORD_VAR))
    {
        return parseVarDeclaration(parser); 
    }
    else if (check(parser, TOKEN_IDENTIFIER))
    {
        return parseAssignment(parser); 
    }
    else if (check(parser, TOKEN_KEYWORD_IF))
    {
        return parseIf(parser);  
    }
    else if (check(parser, TOKEN_KEYWORD_WHILE))
    {
        return parseWhile(parser); 
    }
    else if (check(parser, TOKEN_LBRACE))
    {
        return parseBlock(parser); 
    }

    fprintf(stderr, "Неизвестный оператор (line %d)\n", 
            parser->current_token.line); 
    exit(EXIT_FAILURE);
}


static int parseVarDeclaration(Parser* parser)
{
    char* var_name = strndup(parser->current_token.start, 
                             (size_t)parser->current_token.length); 
    expect(parser, TOKEN_IDENTIFIER, "Ожидался идектификатор");
    expect(parser, TOKEN_EQ, "Ожидалось =");

    int expression_node = parseExpression(parser);
    expect(parser, TOKEN_SEMICOLON, "Ожидалось ';'");

    tree_node_type var_data = {.type = SyntaxNodeType_VAR_DECLARATION};
    int var_node = treeCreateNewNode(parser->ast, var_data);

    int name_node = createIdentifierNode(parser->ast, var_name);
    treeInsertOnLeft(parser->ast, var_node, name_node);
    treeInsertOnRight(parser->ast, var_node, expression_node);

    return var_node;
}


int parseAssignment(Parser* parser)
{
    assert(parser != NULL);

    char* var_name = strndup(parser->current_token.start, 
                             (size_t)parser->current_token.length);
    int name_node = createIdentifierNode(parser->ast, var_name);
    advance(parser);

    expect(parser, TOKEN_EQ, "Ожидалось '='");
    int expression_node = parseExpression(parser);

    expect(parser, TOKEN_SEMICOLON, "Ожидалось ';'");
    tree_node_type assignment_data = {.type = SyntaxNodeType_ASSIGNMENT};
    int assignment_node = treeCreateNewNode(parser->ast, assignment_data); 

    treeInsertOnLeft(parser->ast, assignment_node, name_node);
    treeInsertOnRight(parser->ast, assignment_node, expression_node);

    return assignment_node;
}


static int parseIf(Parser* parser)
{
    assert(parser != NULL);

    advance(parser);
    expect(parser, TOKEN_LPAREN, "Ожидалось '('");

    int condition_node = parseExpression(parser);
    expect(parser, TOKEN_LPAREN, "Ожидалось ')' в if");

    int then_block = parseBlock(parser);
    int else_block = EMPTY_NODE;
    if (check(parser, TOKEN_KEYWORD_ELSE))
    {
        advance(parser);
        else_block = parseBlock(parser); 
    }

    tree_node_type if_data = {.type = SyntaxNodeType_IF};
    int if_node = treeCreateNewNode(parser->ast, if_data);

    treeInsertOnLeft(parser->ast, if_node, condition_node);
    treeInsertOnRight(parser->ast, if_node, then_block);
    if (else_block != EMPTY_NODE)
    {
        treeInsertOnRight(parser->ast, if_node, else_block); 
    }

    return if_node;
}


static int parseBlock(Parser* parser)
{
    assert(parser != NULL);

    expect(parser, TOKEN_LBRACE, "Ожидалось '{'");
    
    tree_node_type block_data = {.type = SyntaxNodeType_BLOCK};
    int block_node = treeCreateNewNode(parser->ast, block_data);

    int previous_node = parser->current_node;
    parser->current_node = block_node;

    while(!check(parser, TOKEN_RBRACE)
       && !check(parser, TOKEN_EOF))
    {
        int statement_node = parseStatement(parser);
        treeInsertOnLeft(parser->ast, block_node, statement_node); 
    }

    expect(parser, TOKEN_RBRACE, "Ожидалось '}'");
    parser->current_node = previous_node;

    return block_node;
}


static int parseExpression(Parser* parser)
{
    assert(parser != NULL);

    return parseLogicalOr(parser);
}


static int parseWhile(Parser* parser)
{
    assert(parser != NULL);

    advance(parser);
    expect(parser, TOKEN_LPAREN, "Ожидалось '('");

    int condition_node = parseExpression(parser);
    expect(parser, TOKEN_RPAREN, "Ожидалось ')'");

    int body_node = parseBlock(parser);

    tree_node_type while_data = {
        .type = SyntaxNodeType_WHILE,
    };

    int while_node = treeCreateNewNode(parser->ast, while_data);
    treeInsertOnLeft(parser->ast, while_node, condition_node);
    treeInsertOnRight(parser->ast, while_node, body_node);

    return while_node;
}





static int parseLogicalOr(Parser* parser)
{
    assert(parser != NULL);

    int left = parseLogicalAnd(parser);
    while (check(parser, TOKEN_OR))
    {
        advance(parser);
        int right = parseLogicalAnd(parser); 
        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = TOKEN_OR
            },
        };

        int operation_node = treeCreateNewNode(parser->ast, operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseLogicalAnd(Parser* parser)
{
    assert(parser != NULL);

    int left = parseEquality(parser);
    while (check(parser, TOKEN_AND))
    {
        advance(parser);
        int right = parseEquality(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = TOKEN_AND
            },
        }; 

        int operation_node = treeCreateNewNode(parser->ast, operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseEquality(Parser* parser)
{
    assert(parser != NULL);

    int left = parseRelationalExpression(parser);
    while (check(parser, TOKEN_EQEQ) 
        || check(parser, TOKEN_BANGEQ))
    {
        TokenType operation = parser->current_token.type;
        advance(parser);

        int right = parseRelationalExpression(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = operation
            }
        };

        int operation_node = treeCreateNewNode(parser->ast, 
                                               operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseRelationalExpression(Parser* parser)
{
    assert(parser != NULL);

    int left = parseAdditiveExpression(parser);
    while (check(parser, TOKEN_LT)
        || check(parser, TOKEN_GT)
        || check(parser, TOKEN_LTEQ)
        || check(parser, TOKEN_GTEQ))
    {
        TokenType operation = parser->current_token.type;
        advance(parser);

        int right = parseAdditiveExpression(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = operation
            }
        };

        int operation_node = treeCreateNewNode(parser->ast, 
                                               operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseAdditiveExpression(Parser* parser)
{
    assert(parser != NULL);

    int left = parseMultiplicativeExpression(parser);
    while (check(parser, TOKEN_PLUS)
        || check(parser, TOKEN_MINUS))
    {
        TokenType operation = parser->current_token.type;
        advance(parser);

        int right = parseMultiplicativeExpression(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = operation
            }
        };

        int operation_node = treeCreateNewNode(parser->ast, 
                                               operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseMultiplicativeExpression(Parser* parser)
{
    assert(parser != NULL);

    int left = parseUnaryExpression(parser);
    while (check(parser, TOKEN_STAR)
        || check(parser, TOKEN_SLASH)
        || check(parser, TOKEN_PERCENT))
    {
        TokenType operation = parser->current_token.type;
        advance(parser);

        int right = parseUnaryExpression(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_BINARY_OPERATION,
            .data = {
                .operation = operation
            }
        };

        int operation_node = treeCreateNewNode(parser->ast, 
                                               operation_data);
        treeInsertOnLeft(parser->ast, operation_node, left);
        treeInsertOnRight(parser->ast, operation_node, right);
        left = operation_node;
    }

    return left;
}


static int parseUnaryExpression(Parser* parser)
{
    assert(parser != NULL);

    if (check(parser, TOKEN_BANG)
     || check(parser, TOKEN_MINUS))
    {
        TokenType operation = parser->current_token.type; 
        advance(parser);

        int operation_node = parseUnaryExpression(parser);

        tree_node_type operation_data = {
            .type = SyntaxNodeType_UNARY_OPERATION,
            .data = {
                .operation = operation
            }
        };
        
        return treeCreateNewNode(parser->ast, operation_data);
    }

    return parsePrimaryExpression(parser);
}


static int parsePrimaryExpression(Parser* parser)
{
    assert(parser != NULL);

    if (check(parser, TOKEN_NUMBER))
    {
        double value = strtod(parser->current_token.start, NULL);
        int node = createNumberNode(parser->ast, value);
        advance(parser);
        return node;
    }
    else if (check(parser, TOKEN_IDENTIFIER))
    {
        char* name = strndup(parser->current_token.start, 
                             (size_t)parser->current_token.length);
        int node = createIdentifierNode(parser->ast, name);
        free(name);
        advance(parser);
        return node;
    }
    else if (check(parser, TOKEN_LPAREN))
    {
        advance(parser);
        int expression = parseExpression(parser);
        expect(parser, TOKEN_RPAREN, "Ожидалось ')'");
        return expression;
    }

    fprintf(stderr, "Не удалось распознать токен\n");
    exit(EXIT_FAILURE);
}


static void advance(Parser* parser)
{
    assert(parser != NULL);

    parser->current_token = nextToken(parser->lexer);
}


static bool check(Parser* parser, TokenType type)
{
    assert(parser != NULL);

    return parser->current_token.type == type;
}


static bool expect(Parser* parser, TokenType type, const char* error_message)
{
    assert(parser        != NULL);
    assert(error_message != NULL);

    if (!check(parser, type))
    {
        fprintf(stderr, "Error: %s (line %d)\n", error_message, parser->current_token.line); 
        exit(EXIT_FAILURE);
    }

    advance(parser);
    return true;
}




static int createNumberNode(Tree* ast, double value)
{
    assert(ast != NULL);

    tree_node_type data = {
        .type = SyntaxNodeType_NUMBER,
        .data = {
            .number = value, 
        },
    };

    return treeCreateNewNode(ast, data);
}

static int createIdentifierNode(Tree* ast, const char* name)
{
    assert(ast  != NULL);
    assert(name != NULL);

    tree_node_type data = {
        .type = SyntaxNodeType_IDENTIFIER,
        .data = {
            .identifier = strdup(name),
        },
    };

    return treeCreateNewNode(ast, data);
}
