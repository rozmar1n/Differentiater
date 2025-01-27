#ifndef recurs_desc_h
#define recurs_desc_h

#include <cstdio>
#include <cassert>
#include <math.h>
#include "../headers/differ.h"

Node* GetExpressionGraph(char* s, int* p);
Node*               GetE(char* s, int* p);
Node*               GetT(char* s, int* p);
Node*               GetU(char* s, int* p);
Node*               GetL(char* s, int* p);
Node*               GetP(char* s, int* p);
Node*               GetN(char* s, int* p);

void    SyntaxError();

#endif //recurs_desc_h