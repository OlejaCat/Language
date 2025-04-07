#ifndef SYNTATIC_ANALYSIS_STRUCT_H
#define SYNTATIC_ANALYSIS_STRUCT_H

typedef enum SyntaxNodeType
{
    SyntaxNodeType_PROGRAM          = 0,
    SyntaxNodeType_VAR_DECLARATION  = 1,
    SyntaxNodeType_ASSIGNMENT       = 2,
    SyntaxNodeType_IF               = 3,
    SyntaxNodeType_ELSE             = 4,
    SyntaxNodeType_WHILE            = 5,
    SyntaxNodeType_BLOCK            = 6,
    SyntaxNodeType_BINARY_OPERATION = 7,
    SyntaxNodeType_UNARY_OPERATION  = 8,
    SyntaxNodeType_NUMBER           = 9,
    SyntaxNodeType_STRING           = 10,
    SyntaxNodeType_IDENTIFIER       = 11,
    SyntaxNodeType_BOOL             = 12,
} SyntaxNodeType;

typedef struct SyntaxNode
{
    SyntaxNodeType type;
    union
    {
        double number;
        char*  string;
        char*  identifier;
        int    operation; 
    } data;
} SyntaxNode;

#endif

