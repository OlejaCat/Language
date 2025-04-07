#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "tree.h"

#include <stdlib.h>

void treePrintData(Tree* tree);
void treePrintDataFromArray(Tree* tree);
void treeDumpToHtm_(Tree* tree);
void treeOpenLogFile_();
void treeLogState_(Tree* tree, const char* original_function);

#ifdef DUMP
    #define treeDumpToHtm(tree_) treeDumpToHtm_(tree_)
#else
    #define treeDumpToHtm(tree_)
#endif

#ifdef LOGGER
    #define treeOpenLogFile() treeOpenLogFile_()
    #define treeLogState(tree_) treeLogState_(tree_, __FUNCTION__)
#else
    #define treeOpenLogFile()
    #define treeLogState(tree_)
#endif

#endif // TREE_DUMP_H
