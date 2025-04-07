#ifndef PRINT_AST_H
#define PRINT_AST_H

#include "tree.h"
#include "syntactic_analysis.h"

void printAST(Tree* tree, int node_index, int indent_level);
void printASTFromRoot(Tree* tree);

#endif
