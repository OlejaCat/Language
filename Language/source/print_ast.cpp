#include "print_ast.h" 
#include <stdio.h>

static void printIndent(int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("   ");
    }
}

static const char* syntaxNodeTypeToString(SyntaxNodeType type) 
{
    static const char* names[] = {
        "PROGRAM",          // 0
        "VAR_DECLARATION",  // 1
        "ASSIGNMENT",       // 2
        "IF",               // 3
        "ELSE",            // 4
        "WHILE",           // 5
        "BLOCK",           // 6
        "BINARY_OPERATION",// 7
        "UNARY_OPERATION", // 8
        "NUMBER",          // 9
        "STRING",          // 10
        "IDENTIFIER",      // 11
        "BOOL"             // 12
    };
    return names[type];
}

static const char* tokenTypeToString(int type) 
{
    static const char* names[] = {
        "EOF",        // 0
        "NUMBER",     // 1
        "STRING",     // 2
        "IDENTIFIER", // 3
        "var",        // 4
        "if",         // 5
        "else",       // 6
        "while",      // 7
        "print",      // 8
        "+",          // 9
        "-",          // 10
        "*",          // 11
        "/",          // 12
        "%",          // 13
        "!",          // 14
        "!=",         // 15
        "=",          // 16
        "==",         // 17
        "<",          // 18
        ">",          // 19
        "<=",         // 20
        ">=",         // 21
        "&&",         // 22
        "||",         // 23
        "(",          // 24
        ")",          // 25
        "{",          // 26
        "}",          // 27
        ";",          // 28
        "ERROR"       // 29
    };
    return names[type];
}

void printAST(Tree* tree, int node_index, int indent_level) 
{
    if (node_index == EMPTY_NODE) return;

    tree_node_type node_data = treeGetNodeData_(tree, node_index LOGGER_PARAMETERS);
    int left = treeGetLeftNode_(tree, node_index LOGGER_PARAMETERS);
    int right = treeGetRightNode_(tree, node_index LOGGER_PARAMETERS);

    printIndent(indent_level);
    printf("%s", syntaxNodeTypeToString(node_data.type));

    switch(node_data.type) 
    {
        case SyntaxNodeType_NUMBER:
            printf(" (%.2f)", node_data.data.number);
            break;
        case SyntaxNodeType_IDENTIFIER:
            printf(" (%s)", node_data.data.identifier);
            break;
        case SyntaxNodeType_STRING:
            printf(" \"%s\"", node_data.data.string);
            break;
        case SyntaxNodeType_BINARY_OPERATION:
        case SyntaxNodeType_UNARY_OPERATION:
            printf(" [%s]", tokenTypeToString(node_data.data.operation));
            break;
        default:
            break;
    }
    printf("\n");

    printAST(tree, left, indent_level + 1);
    printAST(tree, right, indent_level + 1);
}

void printASTFromRoot(Tree* tree) 
{
    if (treeNodesQuantity_(tree LOGGER_PARAMETERS) == 0) 
    {
        printf("AST is empty\n");
        return;
    }
    
    printf("Abstract Syntax Tree:\n");
    printAST(tree, 0, 0); 
}
