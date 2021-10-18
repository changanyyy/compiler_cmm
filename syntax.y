%{
    
#include<stdio.h>    
#include<stdlib.h>
#include<stdarg.h>
#include<assert.h>
#include"lex.yy.c"
#include"include/syntax.h"

  #define YYERROR_VERBOSE 1

%}
%locations

/* declared types*/
%union {
    struct GTNode *node;
    int c;
};




%token <node>        WHITE
%token <node>        DIGIT          LETTER        SEMI        COMMA        ASSIGNOP
%token <node>        RELOP          PLUS          MINUS       STAR         DIV
%token <node>        AND            OR            DOT         NOT          TYPE
%token <node>        LP             RP            LB          RB           LC
%token <node>        RC             STRUCT        RETURN      IF           ELSE
%token <node>        WHILE          INT           FLOAT       ID           REOF   ERROR             

%token <node> error





%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left DIV STAR
%right NOT
%nonassoc L_MINUS
%left DOT
%left  LB RB
%left LP RP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE






%type <node> Program ExtDefList ExtDef ExtDecList Specifier
%type <node> StructSpecifier  OptTag               Tag              VarDec               FunDec 
%type <node> VarList          ParamDec             CompSt           StmtList             Stmt 
%type <node> DefList          Def                  DecList          Dec                  Exp 
%type <node> Args             

%%

Program: 
    ExtDefList                          { $$ = newnode("Program", UNTERMINAL, 1, @$.first_line, $1); root = $$; $$->no = 1;}
    ;
ExtDefList:  
    ExtDef ExtDefList                   { $$ = newnode("ExtDefList", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 1;}
    |                                   { $$ = newnode("ExtDefList", EPSILON, 0,  @$.first_line); $$->no = 2;}    
    ;
ExtDef: 
    Specifier ExtDecList SEMI           { $$ = newnode("ExtDef", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 1;}
    | Specifier SEMI                    { $$ = newnode("ExtDef", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 2;}
    | Specifier FunDec CompSt           { $$ = newnode("ExtDef", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 3;}
    | error SEMI           
    | Specifier error SEMI     

    ;
ExtDecList:
    VarDec                              { $$ = newnode("ExtDecList", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    | VarDec COMMA ExtDecList           { $$ = newnode("ExtDecList", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 2;}
    ;

Specifier:
    TYPE                                { $$ = newnode("Specifier", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    | StructSpecifier                   { $$ = newnode("Specifier", UNTERMINAL, 1, @$.first_line, $1); $$->no = 2;}
    ;
    
StructSpecifier:
    STRUCT OptTag LC DefList RC         { $$ = newnode("StructSpecifier", UNTERMINAL, 5, @$.first_line, $1, $2, $3, $4, $5); $$->no = 1;}
    | STRUCT Tag                        { $$ = newnode("StructSpecifier", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 2;}
    | STRUCT OptTag LC error RC
    ;
OptTag: 
    ID                                  { $$ = newnode("OptTag", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    |                                   { $$ = newnode("ExtDefList", EPSILON, 0, @$.first_line); $$->no = 2;}
    ;
Tag:
    ID                                  {$$ = newnode("Tag", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    ;

VarDec:
    ID                                  { $$ = newnode("VarDec", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    | VarDec LB INT RB                  { $$ = newnode("VarDec", UNTERMINAL, 4, @$.first_line, $1, $2, $3, $4); $$->no = 2;}
    | error RB              
    ;
FunDec:
    ID LP VarList RP                    { $$ = newnode("FunDec", UNTERMINAL, 4, @$.first_line, $1, $2, $3, $4); $$->no = 1;}
    | ID LP RP                          { $$ = newnode("FunDec", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 2;}
    | error RP                  
    ;
VarList:
    ParamDec COMMA VarList              { $$ = newnode("VarList", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 1;}
    | ParamDec                          { $$ = newnode("VarList", UNTERMINAL, 1, @$.first_line, $1); $$->no = 2;}
    ;
ParamDec:
    Specifier VarDec                    { $$ = newnode("ParamDec", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 1;}
    ;

CompSt:
    LC DefList StmtList RC              { $$ = newnode("CompSt", UNTERMINAL, 4, @$.first_line, $1, $2, $3, $4); $$->no = 1;}
    | error RC          
    | LC error            
    ;


    ;
StmtList:
    Stmt StmtList                       { $$ = newnode("StmtList", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 1;}
    |                                   { $$ = newnode("StmtList", EPSILON, 0, @$.first_line); $$->no = 2;}
    ;
Stmt: 
    Exp SEMI                            { $$ = newnode("Stmt", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 1;}
    | CompSt                            { $$ = newnode("Stmt", UNTERMINAL, 1, @$.first_line, $1); $$->no = 2;}
    | RETURN Exp SEMI                   { $$ = newnode("Stmt", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 3;}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = newnode("Stmt", UNTERMINAL, 5, @$.first_line, $1, $2, $3, $4, $5); $$->no = 4;}
    | IF LP Exp RP Stmt ELSE Stmt       { $$ = newnode("Stmt", UNTERMINAL, 7, @$.first_line, $1, $2, $3, $4, $5, $6, $7); $$->no = 5;}
    | WHILE LP Exp RP Stmt              { $$ = newnode("Stmt", UNTERMINAL, 5, @$.first_line, $1, $2, $3, $4, $5); $$->no = 6;}
    | error SEMI   
    | RETURN error SEMI              
    ;


DefList:
    Def DefList                         { $$ = newnode("DefList", UNTERMINAL, 2, @$.first_line, $1, $2); $$->no = 1;}
    |                                   { $$ = newnode("DefList", EPSILON, 0, @$.first_line); $$->no = 2;}
    ; 
Def: 
    Specifier DecList SEMI              { $$ = newnode("Def", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 1;}
    | Specifier error SEMI 
    ;

DecList: 
    Dec                                 { $$ = newnode("DecList", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    | Dec COMMA DecList                 { $$ = newnode("DecList", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 2;}
    ;
Dec: 
    VarDec                              { $$ = newnode("Dec", UNTERMINAL, 1, @$.first_line, $1); $$->no = 1;}
    | VarDec ASSIGNOP Exp               { $$ = newnode("Dec", UNTERMINAL, 3, @$.first_line, $1, $2, $3); $$->no = 2;}
    | VarDec ASSIGNOP error
    ;


Exp:
    Exp ASSIGNOP Exp        { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 1;}
    | Exp AND Exp           { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 2;}
    | Exp OR Exp            { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 3;}
    | Exp RELOP Exp         { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 4;}
    | Exp PLUS Exp          { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 5;}
    | Exp MINUS Exp         { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 6;}
    | Exp STAR Exp          { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 7;}
    | Exp DIV Exp           { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 8;}
    | LP Exp RP             { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 9;}
    | MINUS Exp %prec L_MINUS            { $$ = newnode("Exp", UNTERMINAL, 2, @$.first_line, $1, $2);$$->no = 10;}
    | NOT Exp               { $$ = newnode("Exp", UNTERMINAL, 2, @$.first_line, $1, $2);$$->no = 11;}
    | ID LP Args RP         { $$ = newnode("Exp", UNTERMINAL, 4, @$.first_line, $1, $2, $3, $4);$$->no = 12;}
    | ID LP RP              { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 13;}
    | Exp LB Exp RB         { $$ = newnode("Exp", UNTERMINAL, 4, @$.first_line, $1, $2, $3, $4);$$->no = 14;}
    | Exp DOT ID            { $$ = newnode("Exp", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 15;}
    | ID                    { $$ = newnode("Exp", UNTERMINAL, 1, @$.first_line, $1);$$->no = 16;}
    | INT                   { $$ = newnode("Exp", UNTERMINAL, 1, @$.first_line, $1);$$->no = 17;}
    | FLOAT                 { $$ = newnode("Exp", UNTERMINAL, 1, @$.first_line, $1);$$->no = 18;}
    | error RP 
    | error RB
    ;

Args:
    Exp COMMA Args          { $$ = newnode("Args", UNTERMINAL, 3, @$.first_line, $1, $2, $3);$$->no = 1;}
    | Exp                   { $$ = newnode("Args", UNTERMINAL, 1, @$.first_line, $1); $$->no = 2;}
    ;





%%



// The first arg is the name of the symbol, 
// and the second is the type (it's a terminal symbol or not)
// n is the number of children (0 for terminal symbols)
// remaining args are the children... 

struct GTNode* newnode(char *name, int type, int n, ...){
    
    struct GTNode *newn = malloc(sizeof(struct GTNode));
    strcpy(newn->name, name);
    newn->type = type;
    newn->children = newn->next = NULL;

    //printf("%s\n", newn->name);
    
    va_list argptr;
    va_start(argptr, n);

    newn->first_line = va_arg(argptr, int);

    int i;
    struct GTNode *p;

    if(type > UNTERMINAL)  //it's a terminal symbol
    {
        switch(type){
            case ID:
                strcpy(newn->val.val_string, va_arg(argptr, char *)); break;
            
            case INT:
                newn->val.val_int = va_arg(argptr, long int); break;
            
            case FLOAT:
                newn->val.val_float = va_arg(argptr, double); break;
            
            case TYPE:
            strcpy(newn->val.val_string, va_arg(argptr, char *)); break;

            default: break;
        }

    }
    else if(type == UNTERMINAL)    //it's a unterminal symbol
    {

        // construct a child tree

        newn->children = p = va_arg(argptr, struct GTNode *);
        
        for(i = 1; i < n; i++){
            //printf("%s\n", p->name);
            p->next = va_arg(argptr, struct GTNode *);
            p = p->next;
        }
        p->next = NULL;
    }


    va_end(argptr);
    return newn;
}

void print_tab(int n){
    if(n<=0)return;
    while(n--){
        printf("  ");
    }
}

void print_tree(struct GTNode *r,int base){
    if(error_cnt > 0) return;
    struct GTNode *cptr;
    
    if(!r || r->type == EPSILON){

        //printf("Error: %s\n",r->name);
        return;
    }
    int tabnum = base;
    print_tab(tabnum);

    switch(r->type){
    case UNTERMINAL:
        printf("%s (%d)\n", r->name, r->first_line);
        break;

    case ID:
    case TYPE:
        printf("%s: %s\n", r->name, r->val.val_string);
        break;

    case INT:
        printf("%s: %ld\n", r->name, r->val.val_int);
        break;

    case FLOAT:
        printf("%s: %f\n", r->name, (float)r->val.val_float);
        break;

    default: 
        printf("%s\n", r->name);
        break;
    }

    
    cptr = r->children;
    if(cptr){
        print_tree(cptr, tabnum + 1);
    }

    cptr = r->next;
    while(cptr && cptr->type == EPSILON){
        cptr = cptr->next;
    }
    if(cptr){
        print_tree(cptr, tabnum);
    }

}

void yyerror(const char *msg){

    error_cnt++;
    printf("Error type B at Line %d: %s.\n", yylineno, msg);
}


