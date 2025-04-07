#include "tree.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree_node_structure.h"
#include "tree_dump.h"


// static --------------------------------------------------------------------------------------------------------------


#define ASSERT_LOGGER_ assert(file_name != NULL); \
                       assert(function  != NULL);

#define PASTE_DATA_LOGGER_ tree->file     = file_name; \
                           tree->line     = line_number; \
                           tree->function = function;

static const size_t START_SIZE = 8;
static const size_t SCALE_FACTOR = 2;


// public --------------------------------------------------------------------------------------------------------------


// --------------------------------------- CONSTRUCTOR -----------------------------------------------------------------


Tree* treeCtor_(DUMP_PARAMETERS)
{
#if defined(DUMP) || defined(LOGGER)
    ASSERT_LOGGER_
#endif

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if (tree == NULL)
    {
        return NULL;
    }

    tree->nodes_number   = 0;
    tree->nodes_capacity = START_SIZE;

    tree->nodes_array = (TreeNode*)calloc(START_SIZE, sizeof(TreeNode));

#if defined(DUMP) || defined(LOGGER)
    PASTE_DATA_LOGGER_
#endif

#ifdef LOGGER
    treeOpenLogFile();
    treeLogState(tree);
#endif

    return tree;
}


// ------------------------------------------ RETURN INFORMATION -------------------------------------------------------


size_t treeNodesQuantity_(Tree* tree LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return tree->nodes_number;
}


bool treeIsNodeEnd_(Tree* tree, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(0 <= node_index && node_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    TreeNode node = tree->nodes_array[node_index];

    return node.left_index  == EMPTY_NODE
        && node.right_index == EMPTY_NODE;
}


tree_node_type treeGetNodeData_(Tree* tree, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(0 <= node_index && node_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return tree->nodes_array[node_index].data;
}


// ------------------------------------------ GET ----------------------------------------------------------------------


int treeGetParentNode_(Tree* tree, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(0 <= node_index && node_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return tree->nodes_array[node_index].parent_index;
}


int treeGetLeftNode_(Tree* tree, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(0 <= node_index && node_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return tree->nodes_array[node_index].left_index;
}


int treeGetRightNode_(Tree* tree, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(0 <= node_index && node_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return tree->nodes_array[node_index].right_index;
}


// ------------------------------------------ ADD ----------------------------------------------------------------------


int treeCreateNewNode_(Tree* tree, tree_node_type data LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

    if (tree->nodes_number + 1 > tree->nodes_capacity)
    {
        tree->nodes_capacity *= SCALE_FACTOR;
        tree->nodes_array = (TreeNode*)realloc(tree->nodes_array,
                                               tree->nodes_capacity * sizeof(TreeNode));
        if (tree->nodes_array == NULL)
        {
            return EMPTY_NODE;
        }
    }

    int index = (int)tree->nodes_number;

    tree->nodes_array[index] = {
        .data         = data,
        .node_index   = index,

        .left_index   = EMPTY_NODE,
        .right_index  = EMPTY_NODE,
        .parent_index = EMPTY_NODE,
    };

    tree->nodes_number++;

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return index;
}


int treeInsertOnLeft_(Tree* tree, int node_parent_index, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(-1 <= node_index && node_index < (int)tree->nodes_number);
    assert(-1 <= node_parent_index && node_parent_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

    if (node_index == EMPTY_NODE)
    {
        return EMPTY_NODE;
    }

    tree->nodes_array[node_index].parent_index = node_parent_index;
    if (tree->nodes_array[node_parent_index].left_index != EMPTY_NODE)
    {
        int left_node_index = tree->nodes_array[node_parent_index].left_index;
        tree->nodes_array[node_index].left_index = left_node_index;
        tree->nodes_array[left_node_index].parent_index = node_index;
    }

    tree->nodes_array[node_parent_index].left_index = node_index;

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return node_index;
}


int treeInsertOnRight_(Tree* tree, int node_parent_index, int node_index LOGGER_PARAMETERS)
{
    assert(tree != NULL);
    assert(tree->nodes_array != NULL);
    assert(-1 <= node_index && node_index < (int)tree->nodes_number);
    assert(-1 <= node_parent_index && node_parent_index < (int)tree->nodes_number);

#ifdef LOGGER
    ASSERT_LOGGER_
#endif

    if (node_index == EMPTY_NODE)
    {
        return EMPTY_NODE;
    }

    tree->nodes_array[node_index].parent_index = node_parent_index;
    if (tree->nodes_array[node_parent_index].right_index != EMPTY_NODE)
    {
        int right_node_index = tree->nodes_array[node_parent_index].right_index;
        tree->nodes_array[node_index].right_index = right_node_index;
        tree->nodes_array[right_node_index].parent_index = node_index;
    }

    tree->nodes_array[node_parent_index].right_index = node_index;

#ifdef LOGGER
    PASTE_DATA_LOGGER_

    treeLogState(tree);
#endif

    return node_index;
}


// --------------------------------------- DELETE ----------------------------------------------------------------------





// --------------------------------------- DESTRUCTOR ------------------------------------------------------------------


void treeDtor_(Tree* tree LOGGER_PARAMETERS)
{
#ifdef LOGGER
    ASSERT_LOGGER_
#endif

    if (tree == NULL)
    {
        return;
    }

#ifdef DUMP
    treeDumpToHtm(tree);
#endif
    
#ifdef LOGGER
    PASTE_DATA_LOGGER_
    
    treeLogState(tree);
#endif

    for (size_t i = 0; i < tree->nodes_number; i++) {
        TreeNode* node = &tree->nodes_array[i];
        switch (node->data.type) {
            case SyntaxNodeType_IDENTIFIER:
                free(node->data.data.identifier);
                break;
            case SyntaxNodeType_STRING:
                free(node->data.data.string);
                break;
            default:
                break;
        }
    }

    free(tree->nodes_array);
    tree->nodes_array = NULL;
    tree->nodes_number = 0;
    tree->nodes_capacity = 0;

#if defined(LOGGER) || defined(DUMP)
    tree->file = NULL;
    tree->line = 0;
    tree->function = NULL;
#endif

    free(tree);
}


// static --------------------------------------------------------------------------------------------------------------


#undef ASSERT_LOGGER_
#undef PASTE_DATA_LOGGER_
