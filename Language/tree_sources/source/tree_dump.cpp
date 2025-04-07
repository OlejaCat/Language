#include "tree_dump.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "tree_node_structure.h"


// static --------------------------------------------------------------------------------------------------------------


static void treePrintRecursively(Tree* tree, int node_index);

#if defined(DUMP) || defined(LOGGER)
static void treeMakeGraphvizPng(Tree* tree);
static const char* getLineFromFile(const char* file_name, int line_number);
static void graphvizWriteNodes(FILE* graphviz_file, Tree* tree);

static const char* TEMP_GRAPHVIZ_FILE_PATH = "tmp_.dot";
static const char* GRAPHVIZ_PNG_PATH       = "pictures/dump";

static const size_t FILE_BUFFER_SIZE    = 1024;
static const size_t COMMAND_BUFFER_SIZE = 128;
#endif

#ifdef DUMP
static const char* DUMP_FILE_PATH = "dump.htm";
#endif

#ifdef LOGGER
static void closeLogTreeFile(void);

static const char* LOG_FILE_PATH = "log.htm";

static const char* CTOR_FUNCTION = "treeCtor_";
static const char* DTOR_FUNCTION = "treeDtor_";

static FILE* log_tree_file = NULL;
static size_t logs_writed = 1;
#endif


// public --------------------------------------------------------------------------------------------------------------


void treePrintData(Tree* tree)
{
    assert(tree != NULL);

    treePrintRecursively(tree, 0);
}


void treePrintDataFromArray(Tree* tree)
{
    assert(tree != NULL);

    for (size_t node_index = 0; node_index < tree->nodes_number; node_index++)
    {
        TreeNode node = tree->nodes_array[node_index];

    printf("Index of current node %lu. Value: ", node_index);
    if (node.data.node_type == ArithmeticTreeNodeType_NUMBER)
    {
        printf("%lg\n", node.data.numerical_data);
    }
    else
    {
        printf("%s\n", node.data.function.text);
    }

    printf("\tIndex of parent node = %d\n", node.parent_index);
    printf("\tIndex of left node   = %d\n", node.left_index);
    printf("\tIndex of right node  = %d\n", node.right_index);
    }
}


#ifdef LOGGER
void treeOpenLogFile_()
{
    if (log_tree_file != NULL)
    {
        return;
    }

    log_tree_file = fopen(LOG_FILE_PATH, "w");
    if (log_tree_file == NULL)
    {
        return;
    }

    fprintf(log_tree_file, "<style>\ncode {" \
                           "background-color: #eee;" \
                           "border: 1px solid #999;" \
                           "font-size: 16;" \
                           "display: block;" \
                           "padding: 20px;" \
                            "}\n</style>\n");
    fprintf(log_tree_file, "<pre>\n");

    int return_code_exit = atexit(closeLogTreeFile);
    if (!return_code_exit)
    {
        return;
    }
}


void treeLogState_(Tree* tree, const char* original_function)
{
    assert(tree != NULL);

    fprintf(log_tree_file, "<hr><h2>%lu) Function called: %s in %s:%d in function: %s\n",
                            logs_writed,
                            original_function,
                            tree->file,
                            tree->line,
                            tree->function);
    fprintf(log_tree_file, "<code>%d%s</code></pre><pre></h2><hr>",
                           tree->line,
                           getLineFromFile(tree->file, tree->line));
    fprintf(log_tree_file, "<h3>Tree pointer [%p]\n", tree);
    fprintf(log_tree_file, "\tnumber of nodes   = %lu\n", tree->nodes_number);
    fprintf(log_tree_file, "\tcapacity of nodes = %lu\n", tree->nodes_capacity);

    if (strcmp(CTOR_FUNCTION, original_function) != 0
     && strcmp(DTOR_FUNCTION, original_function) != 0)
    {
        fprintf(log_tree_file, "Image of tree:</h3>\n");

        treeMakeGraphvizPng(tree);

        static char command[COMMAND_BUFFER_SIZE] = {};

        sprintf(command, "mv %s.png %s%lu.png",
                        GRAPHVIZ_PNG_PATH,
                        GRAPHVIZ_PNG_PATH,
                        logs_writed);

        system(command);

        fprintf(log_tree_file, "<img src=\"%s%lu.png\" />",
                               GRAPHVIZ_PNG_PATH,
                               logs_writed);
    }

    logs_writed++;
}
#endif

#ifdef DUMP
void treeDumpToHtm_(Tree* tree)
{
    assert(tree != NULL);

    FILE* output_file = fopen(DUMP_FILE_PATH, "w");
    if (output_file == NULL)
    {
        return;
    }

    fprintf(output_file, "<pre>\n");
    fprintf(output_file, "<hr><h2>Tree created in %s:%d in function: %s\n"
                         "<code>%d%s</code></h2><hr>",
                         tree->file,
                         tree->line,
                         tree->function,
                         tree->line,
                         getLineFromFile(tree->file, tree->line));
    fprintf(log_tree_file, "<h3>Tree pointer [%p]\n", tree);
    fprintf(log_tree_file, "\tnumber of nodes   = %lu\n", tree->nodes_number);
    fprintf(log_tree_file, "\tcapacity of nodes = %lu\n", tree->nodes_capacity);
    fprintf(output_file, "Image of tree:</h3>\n");

    treeMakeGraphvizPng(tree);

    fprintf(output_file, "<img src=\"%s.png\" />", GRAPHVIZ_PNG_PATH);

    fclose(output_file);
}
#endif


// static --------------------------------------------------------------------------------------------------------------


static void treePrintRecursively(Tree* tree, int node_index)
{
    if (node_index == EMPTY_NODE)
    {
        return;
    }

    TreeNode node = tree->nodes_array[node_index];

    printf("Index of current node %d. Value: ", node_index);
    if (node.data.node_type == ArithmeticTreeNodeType_NUMBER)
    {
        printf("%lg\n", node.data.numerical_data);
    }
    else
    {
        printf("%s\n", node.data.function.text);
    }

    printf("\tIndex of parent node = %d\n", node.parent_index);
    printf("\tIndex of left node   = %d\n", node.left_index);
    printf("\tIndex of right node  = %d\n", node.right_index);

    treePrintRecursively(tree, node.left_index);
    treePrintRecursively(tree, node.right_index);
}

#if defined(DUMP) || defined(LOGGER)
static void treeMakeGraphvizPng(Tree* tree)
{
    assert(tree != NULL);

    FILE* graphviz_file = fopen(TEMP_GRAPHVIZ_FILE_PATH, "w");
    if (graphviz_file == NULL)
    {
        return;
    }

    fprintf(graphviz_file, "digraph G\n{");
    fprintf(graphviz_file, "    bgcolor=\"gray20\";\n");
    fprintf(graphviz_file, "    graph [splines=polyline, bgcolor=\"transparent\"]");
    fprintf(graphviz_file, "    node [shape=box, fontname=\"Arial\", fontsize=12, "
                           "fontcolor=white];\n\n");

    graphvizWriteNodes(graphviz_file, tree);

    fprintf(graphviz_file, "}\n");

    fclose(graphviz_file);

    static char command[COMMAND_BUFFER_SIZE] = {};

    sprintf(command, "dot -Tpng %s -o %s.png", TEMP_GRAPHVIZ_FILE_PATH, GRAPHVIZ_PNG_PATH);

    system(command);
}


static void graphvizWriteNodes(FILE* graphviz_file, Tree* tree)
{
    for (int node_index = 0; node_index < (int)tree->nodes_number; node_index++)
    {

        fprintf(graphviz_file,
                "node%d [label=<" \
                "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" BGCOLOR=\"#705833\" ",
                node_index);

        fprintf(graphviz_file, "COLOR=\"white\">\n");

        TreeNode node = tree->nodes_array[node_index];
        fprintf(graphviz_file,
            "<TR><TD COLSPAN=\"2\">parent = %d</TD></TR>\n" \
            "<TR><TD COLSPAN=\"2\">index %d</TD></TR>\n" \
            "<TR><TD COLSPAN=\"2\">data = ",
            node.parent_index,
            node_index);
        if (node.data.node_type == ArithmeticTreeNodeType_NUMBER)
        {
            fprintf(graphviz_file, "%lg\n", node.data.numerical_data);
        }
        else if (node.data.node_type == ArithmeticTreeNodeType_VARIABLE)
        {
            fprintf(graphviz_file, "%s\n", node.data.variable_text);
        }
        else
        {
            fprintf(graphviz_file, "%s\n", node.data.function.text);
        }
        fprintf(graphviz_file, "</TD></TR>\n" \
            "<TR><TD>left = %d</TD><TD>right = %d</TD></TR>\n" \
            "</TABLE>\n" \
            ">];\n",
            node.left_index,
            node.right_index);
    }

    for (int node_index = 0; node_index < (int)tree->nodes_number; node_index++)
    {
        TreeNode node = tree->nodes_array[node_index];
        if (node.parent_index != EMPTY_NODE)
        {
            fprintf(graphviz_file,
                    "node%d -> node%d [color=\"orange\"];\n",
                    // "node%d -> node%d [color=\"red\"];\n",
                    node.parent_index,
                    node_index);
        }
    }
}


static const char* getLineFromFile(const char* file_name, int line_number)
{
    assert(file_name != NULL);

    FILE* file = fopen(file_name, "r");
    if (file == NULL)
    {
        return NULL;
    }

    static char line[FILE_BUFFER_SIZE] = {};
    int current_line = 1;

    while (fgets(line, sizeof(line), file))
    {
        if (current_line == line_number)
        {
            fclose(file);
            return line;
        }

        current_line++;
    }

    fclose(file);
    return NULL;
}
#endif


#ifdef LOGGER
static void closeLogTreeFile(void)
{
    fclose(log_tree_file);
    log_tree_file = NULL;
}
#endif
