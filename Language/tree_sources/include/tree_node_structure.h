#ifndef TREE_NODE_STRUCTURE_H
#define TREE_NODE_STRUCTURE_H

#include <stdlib.h>

#include "tree.h"

typedef struct TreeNode
{
    tree_node_type data;
    int            node_index;

    int            left_index;
    int            right_index;
    int            parent_index;
} TreeNode;

typedef struct Tree
{
    TreeNode* nodes_array;
    size_t    nodes_number;
    size_t    nodes_capacity;

#if defined(LOGGER) || defined(DUMP)
    const char* file;
    int         line;
    const char* function;
#endif
} Tree;

#endif // TREE_NODE_STRUCTURE_H
