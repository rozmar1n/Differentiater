#include "../headers/differ.h"



void* CallocDouble(const double var)
{
    double* var_ptr = (double*)calloc(1, sizeof(double));
    *(var_ptr) = var;
    return (void*)var_ptr;
}

void* CallocString(const char* operation)
{
    char* str_ptr = (char*)calloc(MaxOperationSize, sizeof(char));
    stpcpy(str_ptr, operation);
    return (void*)str_ptr;
}

void* CallocInt(const int operation)
{
    void* operation_ptr  = calloc(1, sizeof(int));
    *(int*)operation_ptr = operation;
    return operation_ptr;
}

Node* MakeNode(NodeTypes type, void* data, Node* left, Node* right)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->node_type   = type;
    new_node->data        = data;
    new_node->child_left  = left;
    new_node->child_right = right;

    return new_node;
}

void DeleteTree(Node* Tree_root)
{
    if (Tree_root->child_left)
    {
        DeleteTree(Tree_root->child_left);
    }
    if (Tree_root->child_right)
    {
        DeleteTree(Tree_root->child_right);
    }

    if(Tree_root->node_type != VAR) 
        free(Tree_root->data);

    free(Tree_root);
}

void  TreeDump(Node* Tree_root, size_t* nPic)
{
    *(nPic) +=1;

    char graphName[128];
    sprintf(graphName, "dump/dotfiles/%lu.dot", *nPic);
    FILE* newGraph = fopen(graphName, "w");
    fprintf(newGraph, "digraph G\n\t" "{\n\t");

    MakeGraphExprNode(Tree_root, newGraph);
    fprintf(newGraph, "\n\t");
    MakeEdgeToChildren(Tree_root, newGraph);
    fprintf(newGraph, "}");
    fclose(newGraph);

    char pictureName[128];
    sprintf(pictureName, "dump/tree_pictures/%lu.png\n", *nPic);

    char buffer[512] = {'\0'};
    sprintf(buffer, "dot -Tpng %s -o %s\n", graphName, pictureName);

    system(buffer);
}

void  MakeEdgeToChildren(Node* node, FILE* newGraph)
{
    if (node == NULL)
    {
        fprintf(stderr, "node is null!!!\n");
        return;
    }
    if(node->child_left)
    {
        fprintf(newGraph, "node%p -> node%p [color = blue;]\n\t",
                           node, node->child_left);
        MakeEdgeToChildren(node->child_left, newGraph);
    }
    if (node->child_right)
    {
        fprintf(newGraph, "node%p -> node%p [color = red;]\n\t",
                           node, node->child_right);
        MakeEdgeToChildren(node->child_right, newGraph);
    }
}

void  MakeGraphExprNode (Node* node, FILE* newGraph)
{
    if (node == NULL)
    {
        fprintf(stderr, "node is null!!!\n");
        return;
    }    
    if(node->data == NULL)
    {
        fprintf(stderr, "data is null\n");
        return;
    }
    
    if (node->node_type == OP)
    {
        fprintf(newGraph, "node%p [shape=Mrecord; label = \" {%p"
                      "| data = %d | left = %p | right = %p }\"];\n\t", 
                       node, node, *(int*)(node->data), node->child_left, node->child_right);
    }
    if (node->node_type == VAR)
    {
        fprintf(newGraph, "node%p [shape=Mrecord; label = \" {%p"
                      "| data = X | left = %p | right = %p }\"];\n\t", 
                       node, node, node->child_left, node->child_right);
    }
    if (node->node_type == NUM)
    {
        fprintf(newGraph, "node%p [shape=Mrecord; label = \" {%p"
                      "| data = %lg | left = %p | right = %p }\"];\n\t", 
                       node, node, *((double*)(node->data)), node->child_left, node->child_right);
    }

    if (node->child_left)
    {
        MakeGraphExprNode(node->child_left, newGraph);
    }
    if (node->child_right)
    {
        MakeGraphExprNode(node->child_right, newGraph);
    }
}

void MakeLaTeXTree (Node* Tree_root, size_t* nPdf)
{
    *(nPdf) = *(nPdf) + 1;

    char TexName[128];
    sprintf(TexName, "dump/tex/%lu.tex", *nPdf);
    FILE* newTeX = fopen(TexName, "w");
    PrintPreamble(newTeX);
    
    fprintf(newTeX, "\\begin{equation} \n");
    PrintfNodeToTeX(Tree_root, newTeX);
    fprintf(newTeX, "\n\\end{equation}\n");
    fprintf(newTeX, "\\end{document}\n");
    fclose(newTeX);
    char buffer[512] = {'\0'};
    sprintf(buffer, "pdflatex -output-directory=dump/pdf -jobname=%lu %s \n", *nPdf, TexName);

    system(buffer);
    system("cd dump/pdf\n rm -rf *.log *.aux *.out\n");
}

void PrintPreamble(FILE* newTeX)
{
    fprintf(newTeX, "\\documentclass[a4paper,12pt]{article} %% добавить leqno в [] для нумерации слева\n");
    fprintf(newTeX, "\\usepackage[a4paper,top=1.3cm,bottom=2cm,left=1.5cm,right=1.5cm,marginparwidth=0.75cm]{geometry}\n");
    fprintf(newTeX, "%% Работа с русским языком\n");
    fprintf(newTeX, "\\usepackage{cmap}          %% поиск в PDF\n");
    fprintf(newTeX, "\\usepackage[warn]{mathtext}     %% русские буквы в формулах\n");
    fprintf(newTeX, "\\usepackage[T2A]{fontenc}      %% кодировка\n");
    fprintf(newTeX, "\\usepackage[utf8]{inputenc}      %% кодировка исходного текста\n");
    fprintf(newTeX, "\\usepackage[english,russian]{babel}  %% локализация и переносы\n");
    fprintf(newTeX, "\\usepackage{physics}\n");
    fprintf(newTeX, "\\usepackage{multirow}\n");
    fprintf(newTeX, "\\usepackage{float}\n");
    fprintf(newTeX, "\\restylefloat{table}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\usepackage{comment}\n");
    fprintf(newTeX, "\\usepackage{graphicx}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\usepackage{wrapfig}\n");
    fprintf(newTeX, "\\usepackage{tabularx}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\usepackage{hyperref}\n");
    fprintf(newTeX, "\\usepackage[rgb]{xcolor}\n");
    fprintf(newTeX, "\\hypersetup{\n");
    fprintf(newTeX, "colorlinks=true,urlcolor=blue\n");
    fprintf(newTeX, "}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "%% Дополнительная работа с математикой\n");
    fprintf(newTeX, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools} %% AMS\n");
    fprintf(newTeX, "\\usepackage{icomma} %% \"Умная\" запятая: $0,2$ --- число, $0, 2$ --- перечисление\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "%% Шрифты\n");
    fprintf(newTeX, "\\usepackage{euscript}   %% Шрифт Евклид\n");
    fprintf(newTeX, "\\usepackage{mathrsfs} %% Красивый матшрифт\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "%% Свои команды\n");
    fprintf(newTeX, "\\DeclareMathOperator{\\sgn}{\\mathop{sgn}}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\date{\\today}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\begin{document}\n");
    fprintf(newTeX, "    \\begin{center}\n");
    fprintf(newTeX, "    \n");
    fprintf(newTeX, "    \\end{center}\n");
    fprintf(newTeX, "  \\vspace{6cm}\n");
    fprintf(newTeX, "  {\\huge\n");
    fprintf(newTeX, "    \\begin{center}\n");
    fprintf(newTeX, "      {\\bf Отчёт о выполнении лабораторной работы 1.4.88}\\\\\n");
    fprintf(newTeX, "      Расчет сферического коня в вакууме\n");
    fprintf(newTeX, "    \\end{center}\n");
    fprintf(newTeX, "  }\n");
    fprintf(newTeX, "  \\vspace{2cm}\n");
    fprintf(newTeX, "  \\begin{center}\n");
    fprintf(newTeX, "    {Остафейчук Роман         \\\n");
    fprintf(newTeX, "      \\vspace{0.2cm}\n");
    fprintf(newTeX, "      Б01-401}\n");
    fprintf(newTeX, "  \\end{center}\n");
    fprintf(newTeX, "  \\vspace{8cm}\n");
    fprintf(newTeX, "  \\begin{center}\n");
    fprintf(newTeX, "    Долгопрудный\\\\\n");
    fprintf(newTeX, "    \\today\n");
    fprintf(newTeX, "  \\end{center}\n");
    fprintf(newTeX, "\\newpage");
    fprintf(newTeX, "\\section*{Введение}\n");
    fprintf(newTeX, "\n");
    fprintf(newTeX, "\\textbf{Цель работы:}\n");
    fprintf(newTeX, "\\begin{itemize}\n");
    fprintf(newTeX, "  \\item Продифференцировать выражение\n");
    fprintf(newTeX, "\\end{itemize}\n");
    fprintf(newTeX, "\\textbf{В работе используются:}\n");
    fprintf(newTeX, "\\begin{itemize}\n");
    fprintf(newTeX, "    \\item Комп\n");
    fprintf(newTeX, "    \\item Копипаст\n");
    fprintf(newTeX, "    \\item Немного смекалки\n");
    fprintf(newTeX, "    \\item Несделанный матан\n");
    fprintf(newTeX, "    \\item отсутствие сна\n");
    fprintf(newTeX, "    \\item баллистические маятники\n");
    fprintf(newTeX, "\\end{itemize}\n");
    fprintf(newTeX, "\\section*{Ход Работы:}\n\t");
}

void PrintfNodeToTeX (Node* node, FILE* newTeX)
{
    switch (node->node_type)
    {
    case OP:
        TypeOperationNode(node, newTeX);
        break;
    
    case NUM:
        fprintf(newTeX, "%lg", *((double*)(node->data)) );
        break;

    case VAR:
        fprintf(newTeX, "X");
        break;

    default:
        break;
    }
}

void TypeOperationNode(Node* node, FILE* newTeX)
{
    if (*(int*)(node->data) == SUM)
    {
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, " + ");
        PrintfNodeToTeX(node->child_right, newTeX);
    }
    if (*(int*)(node->data) == SUB)
    {
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, " - ");
        PrintfNodeToTeX(node->child_right, newTeX);
    }
    if (*(int*)(node->data) == MUL)
    {
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, " \\cdot ");
        if((node->child_right->node_type == OP) && *(int*)(node->child_right->data) < 3)
        {
            fprintf(newTeX, "(");
            PrintfNodeToTeX(node->child_right, newTeX);
            fprintf(newTeX, ")");
        }
        else
        {
            PrintfNodeToTeX(node->child_right, newTeX);
        }
        
    }
    if (*(int*)(node->data) == DIV)
    {
        fprintf(newTeX, "\\frac{");
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, "}{");
        PrintfNodeToTeX(node->child_right, newTeX);
        fprintf(newTeX, "}");
    }
    if (*(int*)(node->data) == POW)
    {
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, " ^{ ");
        PrintfNodeToTeX(node->child_right, newTeX);
        fprintf(newTeX, " }");
    }
    if (*(int*)(node->data) == LOG)
    {
        if (*(double*)(node->child_left->data) - e < 0.001)
        {
            fprintf(newTeX, "\\ln{");
            PrintfNodeToTeX(node->child_right, newTeX);
            fprintf(newTeX, "}");
        }
        else
        {
            fprintf(newTeX, "\\log_{");
            PrintfNodeToTeX(node->child_left, newTeX);
            fprintf(newTeX, "}{");
            PrintfNodeToTeX(node->child_right, newTeX);
            fprintf(newTeX, "}");
        }   
    }
    if(*(int*)(node->data) == SIN)
    {
        fprintf(newTeX, "\\sin(");
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, ")");
    }
    if(*(int*)(node->data) == COS)
    {
        fprintf(newTeX, "\\cos(");
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, ")");
    }
    if(*(int*)(node->data) == TAN)
    {
        fprintf(newTeX, "\\tg(");
        PrintfNodeToTeX(node->child_left, newTeX);
        fprintf(newTeX, ")");
    }
}

Node* CopyNode (const Node* node)
{
    Node* new_node;
    
    if (node->node_type == NUM)
    {
        new_node = MAKE_NUM_NODE(*(double*)(node->data));
    }
    if (node->node_type == OP)
    {
        new_node = Make_OP_NODE(*(int*)(node->data), NULL, NULL);
    }
    if (node->node_type == VAR)
    {
        new_node = Make_VAR_NODE;
    }
    
    if(node->child_left)  new_node->child_left  = CopyNode(node->child_left);
    if(node->child_right) new_node->child_right = CopyNode(node->child_right);
    
    return new_node;
}

Node* DiffNode (Node* node)
{
    if (node->node_type == NUM)  return MAKE_NUM_NODE(0);
    if (node->node_type == VAR) return MAKE_NUM_NODE(1);
    
    if (node->node_type == OP) switch (*(int*)(node->data))
    {
    case SUM:
        return Make_OP_NODE(SUM, DiffNode(node->child_left), DiffNode(node->child_right));
    
    case SUB:
        return Make_OP_NODE(SUB, DiffNode(node->child_left), DiffNode(node->child_right));
    
    case MUL: {
        Node* node_1 = Make_OP_NODE(MUL, DiffNode(node->child_left), CopyNode(node->child_right));
        Node* node_2 = Make_OP_NODE(MUL, CopyNode(node->child_left), DiffNode(node->child_right));
        return Make_OP_NODE(SUM, node_1, node_2);
    }

    case DIV: {
        Node* node_1 = Make_OP_NODE(MUL, DiffNode(node->child_left), CopyNode(node->child_right));
        Node* node_2 = Make_OP_NODE(MUL, CopyNode(node->child_left), DiffNode(node->child_right));
        Node* node_3 = Make_OP_NODE(POW, CopyNode(node->child_right), MAKE_NUM_NODE(2));
        return Make_OP_NODE(DIV, Make_OP_NODE(SUB, node_1, node_2), node_3);
    }

    case LOG: {
        Node* node_1 = Make_OP_NODE(MUL, Make_OP_NODE(DIV, DiffNode(node->child_right), CopyNode(node->child_right)), 
                                         Make_OP_NODE(LOG, MAKE_NUM_NODE(e), CopyNode(node->child_left)));
        
        Node* node_2 = Make_OP_NODE(MUL, Make_OP_NODE(DIV, DiffNode(node->child_left), CopyNode(node->child_left)), 
                                         Make_OP_NODE(LOG, MAKE_NUM_NODE(e), CopyNode(node->child_right)));
        Node* node_3 = Make_OP_NODE(POW, Make_OP_NODE(LOG, MAKE_NUM_NODE(e), CopyNode(node->child_left)), 
                                         MAKE_NUM_NODE(2));    
        return Make_OP_NODE(DIV, Make_OP_NODE(SUB, node_1, node_2), node_3);                                
    }

    case POW: {
        Node* new_deriv = Make_OP_NODE(MUL, CopyNode(node->child_right), Make_OP_NODE(LOG, MAKE_NUM_NODE(e), CopyNode(node->child_left)));
        return Make_OP_NODE(MUL, CopyNode(node), DiffNode(new_deriv));
    }

    case SIN: {
        return Make_OP_NODE(MUL, Make_OP_NODE(COS, CopyNode(node->child_left), NULL), DiffNode(node->child_left));
    }

    case COS: {
        return Make_OP_NODE(MUL, MAKE_NUM_NODE(-1),  Make_OP_NODE(MUL, Make_OP_NODE(SIN, CopyNode(node->child_left), NULL), DiffNode(node->child_left)));
    }
    case TAN: {
        return DiffNode(Make_OP_NODE(DIV, Make_OP_NODE(SIN, node->child_left, NULL), Make_OP_NODE(COS, node->child_left, NULL)));
    }

    default:
        fprintf(stderr, "Wrong Syntax\n");
        return Make_VAR_NODE;
        break;
    }

    return Make_VAR_NODE;
}

size_t CleanNode (Node** node)
{
    int go = 1;
    size_t steps = 3;
    while(go)
    {
        go = 0;
        go += DeleteSomeNodes(node);
        go += MakeTreeSimpler(node);
        MakeLaTeXTree(*node, &steps);
    }
    return steps - 2;
}

int DeleteSomeNodes(Node** node)
{
    int result = 0;

    if((*node)->child_left)  result += DeleteSomeNodes(&((*node)->child_left));
    if((*node)->child_right) result += DeleteSomeNodes(&((*node)->child_right));

    if((*node)->node_type == VAR || (*node)->node_type == NUM) return 0;
    if((*node)->node_type == OP) (*node) = AnalyzeNode(*node, &result);

    return result;
}

Node* AnalyzeNode (Node* node, int* result)
{
    switch (*(int*)(node->data))
    {
        case MUL:
            {            
                if ((node->child_left->node_type == NUM) && fabs(*(double*)(node->child_left->data)) < 0.0001)
                {
                    DeleteTree(node);
                    (*result) += 1;
                    return MAKE_NUM_NODE(0);
                }
                if ((node->child_right->node_type == NUM) && fabs(*(double *)(node->child_right->data)) < 0.0001)
                {
                    DeleteTree(node);
                    (*result) += 1;
                    return MAKE_NUM_NODE(0);
                }
                if ((node->child_left->node_type == NUM) && fabs(*(double*)(node->child_left->data) - 1) < 0.0001)
                {
                    Node* new_graph = CopyNode(node->child_right);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_graph;
                }
                if ((node->child_right->node_type == NUM) && fabs(*(double *)(node->child_right->data) - 1) < 0.0001)
                {
                    Node* new_graph = CopyNode(node->child_left);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_graph;
                }
                return node;
            }
        case SUM:
            {
                if ((node->child_left->node_type == NUM) && fabs(*(double*)(node->child_left->data)) < 0.0001)
                {
                    Node* new_node = CopyNode(node->child_right);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_node;
                }
                if ((node->child_right->node_type == NUM) && fabs(*(double*)(node->child_right->data)) < 0.0001)
                {
                    Node* new_node = CopyNode(node->child_left);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_node;
                }
                return node;
            }
        case SUB:
            {
                if ((node->child_left->node_type == NUM) && fabs(*(double*)(node->child_left->data)) < 0.0001)
                {
                    Node* new_node = CopyNode(node->child_right);
                    DeleteTree(node);
                    (*result) += 1;
                    return Make_OP_NODE(MUL, MAKE_NUM_NODE(-1), new_node);
                }
                if ((node->child_right->node_type == NUM) && fabs(*(double*)(node->child_right->data)) < 0.0001)
                {
                    Node* new_node = CopyNode(node->child_left);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_node;
                }
                return node;
            }
        case DIV:
            {   
                if ((node->child_left->node_type == NUM) && fabs(*(double*)(node->child_left->data)) < 0.0001)
                {
                    DeleteTree(node);
                    (*result) += 1;
                    return MAKE_NUM_NODE(0);
                }
                if ((node->child_right->node_type == NUM) && fabs(*(double*)(node->child_right->data) - 1) < 0.0001)
                {
                    Node* new_node = CopyNode(node->child_left);
                    DeleteTree(node);
                    (*result) += 1;
                    return new_node;
                }
                return node;
            }
        case LOG:
            {
                if ((node->child_left->node_type == NUM || node->child_left->node_type == VAR) && 
                    (node->child_right->node_type == NUM || node->child_right->node_type == VAR) &&
                    fabs(*(double*)(node->child_left->data) - *(double*)(node->child_right->data)) < 0.0001)
                {
                    DeleteTree(node);
                    (*result) += 1;
                    return MAKE_NUM_NODE(1);
                }
                return node;
            }
        case POW:
            {
                if (node->child_left->node_type == NUM && fabs(*(double*)(node->child_left->data)-1) < 0.001)
                {
                    Node* new_graph = MAKE_NUM_NODE(1);
                    DeleteTree(node);
                    *(result) += 1;
                    return new_graph;
                }
                if (node->child_right->node_type == NUM && fabs(*(double*)(node->child_right->data)-1) < 0.001)
                {
                    Node* new_graph = CopyNode(node->child_left);
                    DeleteTree(node);
                    *(result) += 1;
                    return new_graph;
                }
                return node;
            }

        default:
            return node;
            break;
    }
    return node;
}   

int MakeTreeSimpler (Node** node)
{
    int result = 0;

    if((*node)->child_left)  result += MakeTreeSimpler(&((*node)->child_left));
    if((*node)->child_right) result += MakeTreeSimpler(&((*node)->child_right));

    if((*node)->node_type == VAR || (*node)->node_type == NUM) return 0;
    if((*node)->node_type == OP) (*node) = CalcNode(*node, &result);

    return result;
}

Node* CalcNode (Node* node, int* result)
{
    if(node->child_left->node_type == NUM && node->child_right->node_type == NUM) switch (*(int*)(node->data))
    {
    case SUM:
        {
            Node* new_graph = MAKE_NUM_NODE(*(double*)(node->child_left->data) + *(double*)(node->child_right->data));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }

    case SUB:
        {
            Node* new_graph = MAKE_NUM_NODE(*(double*)(node->child_left->data) - *(double*)(node->child_right->data));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }
    
    case MUL:
        {
            Node* new_graph = MAKE_NUM_NODE(*(double*)(node->child_left->data) * *(double*)(node->child_right->data));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }

    case DIV:
        {
            Node* new_graph = MAKE_NUM_NODE(*(double*)(node->child_left->data) / *(double*)(node->child_right->data));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }

    case POW:
        {
            Node* new_graph = MAKE_NUM_NODE(pow(*(double*)(node->child_left->data), *(double*)(node->child_right->data)));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }

    case LOG:
        {
            Node* new_graph = MAKE_NUM_NODE(log(*(double*)(node->child_left->data)) / log(*(double*)(node->child_right->data)));
            DeleteTree(node);
            (*result)++;
            return new_graph;
        }
        
    default:
        fprintf(stderr, "something had gone wrong\n");
        break;
    }
    return node;
}