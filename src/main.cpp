#include "../headers/differ.h"
#include "../headers/RecursDesc.h"

int main(void)
{
    char s[] = {"x^2+2*x*85+s(x^l(2.718281828)(x))$"};
    int p = 0;

    size_t nPic =0;

    Node* expression = GetExpressionGraph(s, &p);
    MakeLaTeXTree(expression, &nPic);

    Node* derivative = DiffNode(expression);
    MakeLaTeXTree(derivative, &nPic);
    TreeDump(derivative, &nPic);

    CleanNode(&derivative);
    nPic += 4;
    TreeDump(derivative, &nPic);

    DeleteTree(expression);
    DeleteTree(derivative);
}