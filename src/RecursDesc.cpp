#include "../headers/RecursDesc.h"

void SyntaxError()
{
    assert(0);
}

Node* GetExpressionGraph(char* s, int* p)
{
    Node* val = GetE(s, p);
    if (s[*p] != '$')
    {
        fprintf(stderr, "\n\tSYMBOL: %c\n",s[*p]);
        SyntaxError();
    }
    (*p)++;
    return val;
}

Node* GetE(char* s, int* p)
{
    Node* val_1 = GetT(s, p);
    while(s[*p] == '+'|| s[*p] == '-')
    {
        int op = s[*p];
        (*p)++;
        Node* val_2 = GetT(s, p);
        if (op == '+')
        {
            val_1 = Make_OP_NODE(SUM, val_1, val_2);
        }
        else
            val_1 = Make_OP_NODE(SUM, val_1, val_2);
    }
    return val_1;
}

Node* GetT(char* s, int* p)
{
    Node* val = GetU(s, p);
    while(s[*p] == '*'|| s[*p] == '/')
    {
        int op = s[*p];
        (*p)++;
        Node* val_2 = GetU(s, p);
        if (op == '*')
        {
            val = Make_OP_NODE(MUL, val, val_2);
        }
        else   
            val = Make_OP_NODE(DIV, val, val_2); 
    }
    return val;
}

Node* GetU(char* s, int* p)
{
    Node* val = GetL(s, p);
    if (s[*p] == '^')
    {
        (*p)++;
        Node* val_2 = GetL(s, p);
        val = Make_OP_NODE(POW, val, val_2);
    }
    return val;
}

Node* GetL(char* s, int* p)
{
    if (s[*p] == 'l')
    {
        (*p) += 2;
        Node* val_1 = GetE(s, p);
        (*p) +=2;
        Node* val_2 = GetE(s, p);
        (*p) +=1;

        return Make_OP_NODE(LOG, val_1, val_2);
    }
    else if(s[*p] == 's')
    {
        (*p) += 2;
        Node* val_1 = GetE(s, p);
        (*p) += 1;

        return Make_OP_NODE(SIN, val_1, NULL);
    }
    else if(s[*p] == 'c')
    {
        (*p) += 2;
        Node* val_1 = GetE(s, p);
        (*p) += 1;

        return Make_OP_NODE(COS, val_1, NULL);
    }
    else if(s[*p] == 't')
    {
        (*p) += 2;
        Node* val_1 = GetE(s, p);
        (*p) += 1;

        return Make_OP_NODE(TAN, val_1, NULL);
    }
    else
    {
        return GetP(s, p);
    }
}

Node* GetP(char* s, int* p)
{
    if(s[*p] == '(')
    {
        (*p)++;
        Node* val = GetE(s, p);
        if(s[*p] != ')')
            SyntaxError();
        (*p)++;
        return val;
    }
    else
    {
        if (s[*p] == 'x')
        {
            (*p)++;
            return Make_VAR_NODE;
        }
        else
        {
            return GetN(s, p);
        }
    }
}

Node* GetN(char* s, int* p)
{
    double val = 0;
    double int_part = 0;
    double old_p = *p;
    while(('0'<= s[*p] && s[*p]<='9'))
    {
        int_part = int_part*10 + s[*p] - '0';
        (*p)++;
    }
    val = int_part;

    double digits_after_point = 0;
    double frac_part = 0;
    if ((s[*p] == '.'))
    {
        if (*p == old_p)
        {
            return MAKE_NUM_NODE(NAN);
        }
        (*p)++;
        while(('0'<= s[*p] && s[*p]<='9'))
        {
            frac_part = frac_part*10 + s[*p] - '0';
            digits_after_point++;
            (*p)++;
        }
        val += frac_part/(pow(10,digits_after_point));
    }
    
    if (old_p == *p)
    {
        return MAKE_NUM_NODE(NAN);
    }
    return MAKE_NUM_NODE(val);
}

