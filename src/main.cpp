#include "../headers/differ.h"
#include "../headers/RecursDesc.h"

int main(void)
{
    char s[] = {"(s(x))^(c(x))+(c(x))^(s(x))$"};
    int p = 0;

    size_t nPic =0;
    size_t nPdf =12122;

    Node* expression = GetExpressionGraph(s, &p);
    MakeLaTeXTree(expression, &nPic);

    Node* derivative = MakeDiffLatex(expression, &nPdf);
    MakeLaTeXTree(derivative, &nPic);
    TreeDump(derivative, &nPic);

    CleanNode(&derivative);
    nPic += 10;
    TreeDump(derivative, &nPic);

    DeleteTree(expression);
    DeleteTree(derivative);
}