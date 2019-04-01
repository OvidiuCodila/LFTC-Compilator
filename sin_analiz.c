#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vars.h"


Token *crtTk;

int consume(int code);
int declStruct();
int declVar();
int typeBase();
int arrayDecl();
int typeName();
int declFunc();
int funcArg();
int stm();
int stmCompund();
int expr();
int exprAssign();
int exprOr();
int exprOrPrim();
int exprAnd();
int exprAndPrim();
int exprEq();
int exprEqPrim();
int exprRel();
int exprRelPrim();
int exprAdd();
int exprAddPrim();
int exprMul();
int exprMulPrim();
int exprCast();
int exprUnary();
int exprPostfix();
int exprPostfixPrim();
int exprPrimary();


int consume(int code)
{
    printf("#consume -> %s ->",convertAtomsName(code));
    if(crtTk->code == code)
    {
        printf("Consumat\n");
        crtTk = crtTk->next;
        return 1;
    }
    else
    {
        printf("Neconsumat -> avem: %s \n",convertAtomsName(crtTk->code));
        return 0;
    }
}

int exprAssign()
{
    Token *startTk = crtTk;
    printf("#exprAssign() -> %s\n",convertAtomsName(crtTk->code));

    if(exprUnary())
    {
        if(consume(ASSIGN))
            if(exprAssign()) return 1;
    }
    if(exprOr()) return 1;
    
    crtTk = startTk;
    //printf("#exprAssign() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprPrimary()
{
    Token *startTk = crtTk;
    printf("#exprPrimary() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(ID))
    {
        consume(LPAR);
        expr();
        for(;;)
        {
            if(consume(COMMA))
            {
                if(expr()) {}
            }
            else break;
        }
        consume(RPAR);
        return 1;
    }
    if(consume(CT_INT)) return 1;
    if(consume(CT_REAL)) return 1;
    if(consume(CT_CHAR)) return 1;
    if(consume(CT_STRING)) return 1;
    if(consume(LPAR))
    {
        if(expr())
            if(consume(RPAR))
                return 1;
            else tokenError(crtTk, "Missing )");
    }
    
    crtTk = startTk;
    //printf("#exprPrimary() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprPostfixPrim()
{
    Token *startTk = crtTk;
    printf("#exprPostfixPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(LBRACKET))
    {
        if(expr())
            if(consume(RBRACKET))
            {
                if(exprPostfixPrim())
                    return 1;
            }else tokenError(crtTk, "Missing ]");
    }
    if(consume(DOT))
    {
        if(consume(ID))
        {
            if(exprPostfixPrim())
                return 1;
        }else tokenError(crtTk, "Missing ID");
    }

    crtTk = startTk;
    //printf("#exprPostfixPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprPostfix()
{
    Token *startTk = crtTk;
    printf("#exprPostfix() -> %s\n",convertAtomsName(crtTk->code));

    if(exprPrimary())
        if(exprPostfixPrim())
            return 1;

    crtTk = startTk;
    //printf("#exprPostfix() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprUnary()
{
    Token *startTk = crtTk;
    printf("#exprUnary() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(SUB) || consume(NOT))
    {
        if(exprUnary())
            return 1;
    }
    if(exprPostfix()) return 1;

    crtTk = startTk;
    //printf("#exprUnary() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprCast()
{
    Token *startTk = crtTk;
    printf("#exprCast() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(LPAR))
    {
        if(typeName())
        {
            if(consume(RPAR))
            {
                if(exprCast()) return 1;
                else tokenError(crtTk, "Idk what error is this. Cast something");
            }else tokenError(crtTk, "Missing ) for cast");
        }else tokenError(crtTk, "Invalid type name for cast");
    }
    if(exprUnary()) return 1;

    crtTk = startTk;
    //printf("#exprCast() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprMulPrim()
{
    Token *startTk = crtTk;
    printf("#exprMulPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(MUL) || consume(DIV))
        if(exprCast())
            if(exprMulPrim())
                return 1;
    
    crtTk = startTk;
    //printf("#exprMulPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprMul()
{
    Token *startTk = crtTk;
    printf("#exprMul() -> %s\n",convertAtomsName(crtTk->code));

    if(exprCast())
        if(exprMulPrim())
            return 1;
    
    crtTk = startTk;
    //printf("#exprMul() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprAddPrim()
{
    Token *startTk = crtTk;
    printf("#exprAddPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(ADD) || consume(SUB))
        if(exprMul())
            if(exprAddPrim())
                return 1;
    
    crtTk = startTk;
    //printf("#exprAddPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprAdd()
{
    Token *startTk = crtTk;
    printf("#exprAdd() -> %s\n",convertAtomsName(crtTk->code));

    if(exprMul())
        if(exprAddPrim())
            return 1;
    
    crtTk = startTk;
    //printf("#exprAdd() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprRelPrim()
{
    Token *startTk = crtTk;
    printf("#exprRelPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ))
        if(exprAdd())
            if(exprRelPrim())
                return 1;
    
    crtTk = startTk;
    //printf("#exprRelPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprRel()
{
    Token *startTk = crtTk;
    printf("#exprRel() -> %s\n",convertAtomsName(crtTk->code));

    if(exprAdd())
        if(exprRelPrim())
            return 1;
    
    crtTk = startTk;
    //printf("#exprRel() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprOrPrim()
{
    Token *startTk = crtTk;
    printf("#exprOrPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(OR))
        if(exprAnd())
            if(exprOrPrim())
                return 1;
    
    crtTk = startTk;
    //printf("#exprOrPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprOr()
{
    Token *startTk = crtTk;
    printf("#exprOr() -> %s\n",convertAtomsName(crtTk->code));

    if(exprAnd())
        if(exprOrPrim())
            return 1;
    
    crtTk = startTk;
    //printf("#exprOr() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprEqPrim()
{
    Token *startTk = crtTk;
    printf("#exprEqPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(EQUAL) || consume(NOTEQ))
        if(exprRel())
            if(exprEqPrim())
                return 1;
    
    crtTk = startTk;
    //printf("#exprEqPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprEq()
{
    Token *startTk = crtTk;
    printf("#exprEq() -> %s\n",convertAtomsName(crtTk->code));

    if(exprRel())
        if(exprEqPrim())
            return 1;
    
    crtTk = startTk;
    //printf("#exprEq() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int exprAndPrim()
{
    Token *startTk = crtTk;
    printf("#exprAndPrim() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(AND))
        if(exprEq())
            if(exprAndPrim())
                return 1;

    crtTk = startTk;
    //printf("#exprAndPrim() -> %s\n",convertAtomsName(crtTk->code));
    return 1;
}

int exprAnd()
{
    Token *startTk = crtTk;
    printf("#exprAnd() -> %s\n",convertAtomsName(crtTk->code));

    if(exprEq())
        if(exprAndPrim())
            return 1;

    crtTk = startTk;
    //printf("#exprAnd() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int stm()
{
    Token *startTk = crtTk;
    printf("#stm() -> %s\n",convertAtomsName(crtTk->code));

    if(stmCompund()) return 1;
    if(consume(IF))
    {
        if(consume(LPAR))
        {
            if(expr())
            {
                if(consume(RPAR))
                {
                    if(stm())
                    {
                        if(consume(ELSE))
                            if(!stm()) tokenError(crtTk,"Missing else statement");
                        return 1;
                    }else tokenError(crtTk, "Missing action after if statement");
                }else tokenError(crtTk, "Missing ) after if statement");
            }else tokenError(crtTk, "Missing expression inside if statement");
        }else tokenError(crtTk, "Missing ( after IF statement");
    }
    if(consume(WHILE))
    {
        if(consume(LPAR))
        {
            if(expr())
            {
                if(consume(RPAR))
                {
                    if(stm())
                        return 1;
                    else tokenError(crtTk, "Missing while statement");
                }else tokenError(crtTk, "Missing ) after while statement");
            }else tokenError(crtTk, "Missing expression inside while statement");
        }else tokenError(crtTk, "Missing ( after while statement");
    }
    if(consume(FOR))
    {
        if(consume(LPAR))
        {
            expr();
            if(consume(SEMICOLON))
            {
                expr();
                if(consume(SEMICOLON))
                {
                    expr();
                    if(consume(RPAR))
                    {
                        if(stm())
                            return 1;
                        else tokenError(crtTk, "Missing for statement");
                    }else tokenError(crtTk, "Missing ) after for statement");
                }else tokenError(crtTk, "Missing ; in for statement");
            }else tokenError(crtTk, "Missing ; in for statement");
        }else tokenError(crtTk, "Missing ( after for statement");
    }
    if(consume(BREAK))
    {
        if(consume(SEMICOLON))
            return 1;
        else tokenError(crtTk, "Missing ; after break statement");
    }
    if(consume(RETURN))
    {
        expr();
        if(consume(SEMICOLON))
            return 1;
        else tokenError(crtTk, "Missing ; after return statement");
    }
    if(expr())
    {
        if(consume(SEMICOLON))
            return 1;
        else tokenError(crtTk, "Missing ; at the end of expression");
    }

    crtTk = startTk;
    //printf("#stm() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int stmCompund()
{
    Token *startTk = crtTk;
    printf("#stmCompund() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(LACC))
    {
        for(;;)
        {
            if(declVar()) {}
            else if(stm()) {}
            else break;
        }
        if(consume(RACC))
            return 1;
        else tokenError(crtTk, "Missing } at the end of statement");
    }

    crtTk = startTk;
    //printf("#stmCompund() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int expr()
{
    Token *startTk = crtTk;
    printf("#expr() -> %s\n",convertAtomsName(crtTk->code));

    if(exprAssign()) return 1;
    
    crtTk = startTk;
    //printf("#expr() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int typeBase()
{
    Token *startTk = crtTk;
    printf("#typeBase() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(INT)) return 1;
    if(consume(DOUBLE)) return 1;
    if(consume(CHAR)) return 1;
    if(consume(STRUCT))
    {
        if(consume(ID)) return 1;
        else tokenError(crtTk, "Missing structure ID");
    }

    crtTk = startTk;
    //printf("#typeBase() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int arrayDecl()
{
    Token *startTk = crtTk;
    printf("#arrayDecl() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(LBRACKET))
    {
        expr();
        if(consume(RBRACKET))
            return 1;
        else tokenError(crtTk, "Missing ] in array declaration");
    }

    crtTk = startTk;
    //printf("#arrayDecl() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int typeName()
{
    Token *startTk = crtTk;
    printf("#typeName() -> %s\n",convertAtomsName(crtTk->code));

    if(typeBase())
    {
        arrayDecl();
        return 1;
    }

    crtTk = startTk;
    //printf("#typeName() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int funcArg()
{
    Token *startTk = crtTk;
    printf("#funcArg() -> %s\n",convertAtomsName(crtTk->code));

    if(typeBase())
        if(consume(ID))
        {
            arrayDecl();
            return 1;
        }else tokenError(crtTk, "Missing variable name");

    crtTk = startTk;
    //printf("#funcArg() -> %s\n",convertAtomsName(crtTk->code));
    return 0;
}

int declFunc()
{
    Token *startTk = crtTk;
    printf("#declFunc() -> %s\n",convertAtomsName(crtTk->code));

    if(typeBase())
    {
        consume(MUL);
    }
    else if(consume(VOID)) {}
    else { crtTk = startTk;  return 0; }

    if(consume(ID))
    {
        if(consume(LPAR))
        {
            funcArg();
            for(;;)
            {
                if(consume(COMMA))
                {
                    if(funcArg()) {}
                }
                else break;
            }
            if(consume(RPAR))
            {
                if(stmCompund())
                    return 1;
                else tokenError(crtTk, "Function body missing");
            }else tokenError(crtTk, "Missing ) in function declaration");
        }else tokenError(crtTk, "Missing ( in function declaration");
    }else tokenError(crtTk, "Missing function ID");

    crtTk = startTk;
    return 0;
}

int declVar()
{
    Token *startTk = crtTk;
    printf("#declVar() -> %s\n",convertAtomsName(crtTk->code));

    if(typeBase())
        if(consume(ID))
        {
            arrayDecl();
            for(;;)
            {
                if(consume(COMMA))
                {
                    if(consume(ID))
                        arrayDecl();
                    else tokenError(crtTk, "Missing variable ID after comma");
                }
                else break;
            }
            if(consume(SEMICOLON))
                return 1;
            else tokenError(crtTk, "Missing ; at the end of line");
        } else tokenError(crtTk, "Missing variable ID");

    crtTk = startTk;
    return 0;
}

int declStruct()
{
    Token *startTk = crtTk;
    printf("#declStruct() -> %s\n",convertAtomsName(crtTk->code));

    if(consume(STRUCT))
        if(consume(ID))
        {
            if(consume(LACC))
            {
                for(;;)
                {
                    if(declVar()) {}
                    else break;
                }
                if(consume(RACC))
                {
                    if(consume(SEMICOLON))
                        return 1;
                    else tokenError(crtTk,"Missing ; at the end of line");
                }else tokenError(crtTk,"Mising } at the end of statement");
            }else tokenError(crtTk,"Missing { at the begining of statement");
        }else tokenError(crtTk,"Missing structure id");

    crtTk = startTk;
    return 0;
}

int unit(Token *tkn)
{
    crtTk = tkn;
    for(;;)
    {
        if(declStruct()) {}
        else if(declFunc()) {}
        else if(declVar()) {}
        else break;
    }

    if(consume(END)) return 1;
    else return 0;
}