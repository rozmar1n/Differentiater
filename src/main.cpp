#include "../headers/differ.h"
#include "../headers/RecursDesc.h"

int main(void)
{
    char s[] = {"x^2+2*x*85-32*(x+95*x-5)$"};
    int p = 0;

    size_t nPic =0;

    Node* expression = GetExpressionGraph(s, &p);
    MakeLaTeXTree(expression, &nPic);

    Node* derivative = DiffNode(expression);
    MakeLaTeXTree(derivative, &nPic);
    TreeDump(derivative, &nPic);

    CleanNode(&derivative);

    DeleteTree(expression);
    DeleteTree(derivative);
}