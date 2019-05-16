#define true 1
#define SAFEALLOC(var,Type) if((var=(Type*)malloc(sizeof(Type)))==NULL)error("not enough memory");
 
 
typedef struct _Token{
    int code;
    union{
        char *text;
        long int c;
        double d;
    };
    int line;
    struct _Token *next;
    struct _Token *prev;
}Token;
 
 
enum Atoms{END, ID, CT_INT, CT_REAL, CT_STRING, CT_CHAR, COMMA, SEMICOLON, LPAR, RPAR,
        LBRACKET, RBRACKET, LACC, RACC, ADD, SUB, MUL, DIV, DOT, AND, OR, NOT,
        ASSIGN, EQUAL, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ, BREAK, CHAR, DOUBLE,
        ELSE, FOR, IF, INT, RETURN, STRUCT, VOID, WHILE};
 
char *pCrtCh;
int line;
Token *tokens, *lastToken;
 
 
void error(char *msj)
{
    printf(msj);
    printf("\n");
    exit(-1);
}
 
void tokenError(Token *tk, char *msj)
{
    printf("Error! on line %d: ",tk->line);
    printf(msj);
    printf("\n");
    exit(-1);
}
 
char *convertAtomsName(int code)
{
    switch(code)
    {
        case END: { return "END"; break; }
        case ID: { return "ID"; break; }
        case CT_INT: { return "CT_INT"; break; }
        case CT_REAL: { return "CT_REAL"; break; }
        case CT_STRING: { return "CT_STRING"; break; }
        case CT_CHAR: { return "CT_CHAR"; break; }
        case COMMA: { return "COMMA"; break; }
        case SEMICOLON: { return "SEMICOLON"; break; }
        case LPAR: { return "LPAR"; break; }
        case RPAR: { return "RPAR"; break; }
        case ASSIGN: { return "ASSIGN"; break; }
        case INT: { return "INT"; break; }
        case LBRACKET: { return "LBRACKET"; break; }
        case RBRACKET: { return "RBRACKET"; break; }
        case LACC: { return "LACC"; break; }
        case RACC: { return "RACC"; break; }
        case ADD: { return "ADD"; break; }
        case SUB: { return "SUB"; break; }
        case MUL: { return "MUL"; break; }
        case DIV: { return "DIV"; break; }
        case DOT: { return "DOT"; break; }
        case AND: { return "AND"; break; }
        case OR: { return "OR"; break; }
        case NOT: { return "NOT"; break; }
        case EQUAL: { return "EQUAL"; break; }
        case NOTEQ: { return "NOTEQ"; break; }
        case LESS: { return "LESS"; break; }
        case LESSEQ: { return "LESSEQ"; break; }
        case GREATER: { return "GREATER"; break; }
        case GREATEREQ: { return "GREATEREQ"; break; }
        case BREAK: { return "BREAK"; break; }
        case CHAR: { return "CHAR"; break; }
        case DOUBLE: { return "DOUBLE"; break; }
        case ELSE: { return "ELSE"; break; }
        case FOR: { return "FOR"; break; }
        case IF: { return "IF"; break; }
        case RETURN: { return "RETURN"; break; }
        case STRUCT: { return "STRUCT"; break; }
        case VOID: { return "VOID"; break; }
        case WHILE: { return "WHILE"; break; }
        default: { error("Wrong code"); break; }
    }
}