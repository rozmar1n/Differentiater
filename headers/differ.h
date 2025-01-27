#ifndef DIFFER_H
#define DIFFER_H

#include <cstdio>
#include <cstring>
#include <cstdbool>
#include <cstdlib>
#include <math.h>

#define MAKE_OP_NODE(op, Node1, Node2)  MakeNode(t_OP,  CallocInt(op), Node1, Node2)
#define MAKE_NUM_NODE(double)           MakeNode(t_NUM, CallocDouble(double), NULL, NULL)
#define Make_VAR_NODE                   MakeNode(t_VAR, CallocDouble(0xdeadbeef), NULL, NULL)

const size_t MaxOperationSize = 32;
const double e                = 2.718281828;

enum NodeTypes
{
    t_OP  = 1,
    t_NUM = 2,
    t_VAR = 3
};

enum Operations 
{
    SUM = 1, 
    SUB = 2, 
    MUL = 3, 
    DIV = 4,
    POW = 5,
    LOG = 6,
    SIN = 7,
    COS = 8,
    TAN = 9
};

struct Node
{
    NodeTypes node_type;
    void*     data;
    Node*     child_left;
    Node*     child_right;
};

void* CallocDouble      (const double var);
void* CallocString      (const char* operation);
void* CallocInt         (const int operation);
Node* MakeNode          (NodeTypes type, void* data, Node* left, Node* right);
void  DeleteTree        (Node* Tree_root);

void  TreeDump          (Node* Tree_root, size_t* nPic);
void  MakeEdgeToChildren(Node* node, FILE* newGraph);
void  MakeGraphExprNode (Node* node, FILE* newGraph);

void  MakeLaTeXTree      (Node* Tree_root, size_t* nPdf);
void  PrintPreamble      (FILE* newTeX);
void  PrintfNodeToTeX    (Node* node, FILE* newTeX);
void  TypeOperationNode  (Node* node, FILE* newTeX);
Node* MakeDiffLatex      (Node* node, size_t* nPdf);
Node* LaTeXDiffNode      (Node* node, FILE* newTeX);

Node* DiffNode          (Node* node);
Node* CopyNode          (const Node* node);

size_t   CleanNode      (Node** node);
int      DeleteSomeNodes(Node** node);
int      MakeTreeSimpler(Node** node);
Node*    AnalyzeNode    (Node* node, int* result);
Node*    CalcNode       (Node* node, int* result);
          
#endif /*DIFFER_H*/
