

%{

#include<stdio.h>
#include<stdlib.h>
#include"syntax.tab.h"
#include"include/syntax.h"



int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;




%}


%option yylineno

r_white     [ \t\n]
r_digit     [0-9]
r_letter    [a-zA-Z]
r_semi      ";"
r_comma     ","
r_assignop  "="
r_relop     ">"|"<"|">="|"<="|"=="|"!="
r_plus      "+"
r_minus     "-"
r_star      "*"
r_div       "/"
r_and       "&&"
r_or        "||"
r_dot       "."
r_not       "!"
r_lp        "("             
r_rp        ")"            
r_lb        "["           
r_rb        "]"          
r_lc        "{"               
r_rc        "}"
r_struct    "struct"
r_return    "return"
r_if        "if"
r_else      "else"
r_while     "while"
r_int       ([1-9]{r_digit}*)|("0"[xX]({r_letter}|{r_digit})*)|("0"{r_digit}+)|0
r_float     ({r_digit}+\.{r_digit}+)|((({r_digit}*\.{r_digit}+)|({r_digit}+\.{r_digit}*))[Ee][+-]?{r_digit}+)
r_type      ("int")|("float")
r_id        (_|{r_letter})(_|{r_letter}|{r_digit})*
r_eof       "<<EOF>>"
r_other     .

r_comment   ("//"[^\n]*)|("/*"([^\*]|(\*)*[^\*/])*(\*)*"*/")
r_err_comment "/*"([^\*]|(\*)*[^\*/])*(\*)*

%%

{r_white}       { }
{r_semi}        { yylval.node = newnode("SEMI", SEMI, 0, yylineno);           return SEMI; }
{r_comma}       { yylval.node = newnode("COMMA", COMMA, 0, yylineno);         return COMMA; }
{r_assignop}    { yylval.node = newnode("ASSIGNOP", ASSIGNOP, 0, yylineno);   return ASSIGNOP; }
{r_relop}       { yylval.node = newnode("RELOP", RELOP, 0, yylineno, yytext); return RELOP; }
{r_plus}        { yylval.node = newnode("PLUS", PLUS, 0, yylineno);           return PLUS; }
{r_minus}       { yylval.node = newnode("MINUS", MINUS, 0, yylineno);         return MINUS; }
{r_star}        { yylval.node = newnode("STAR", STAR, 0, yylineno);           return STAR; }
{r_div}         { yylval.node = newnode("DIV", DIV, 0, yylineno);             return DIV; }
{r_and}         { yylval.node = newnode("AND", AND, 0, yylineno);             return AND;}
{r_or}          { yylval.node = newnode("OR", OR, 0, yylineno);               return OR; }
{r_dot}         { yylval.node = newnode("DOT", DOT, 0, yylineno);             return DOT; }
{r_not}         { yylval.node = newnode("NOT", NOT, 0, yylineno);             return NOT; }
{r_lp}          { yylval.node = newnode("LP", LP, 0, yylineno);               return LP; }
{r_rp}          { yylval.node = newnode("RP", RP, 0, yylineno);               return RP; }
{r_lb}          { yylval.node = newnode("LB", LB, 0, yylineno);               return LB; }
{r_rb}          { yylval.node = newnode("RB", RB, 0, yylineno);               return RB; }
{r_lc}          { yylval.node = newnode("LC", LC, 0, yylineno);               return LC; }    
{r_rc}          { yylval.node = newnode("RC", RC, 0, yylineno);               return RC; }
{r_struct}      { yylval.node = newnode("STRUCT", STRUCT, yylineno, 0);       return STRUCT; }
{r_return}      { yylval.node = newnode("RETURN", RETURN, yylineno, 0);       return RETURN; }
{r_if}          { yylval.node = newnode("IF", IF, 0, yylineno);               return IF; }
{r_else}        { yylval.node = newnode("ELSE", ELSE, 0, yylineno);           return ELSE; }
{r_while}       { yylval.node = newnode("WHILE", WHILE, 0, yylineno);         return WHILE; }
{r_int}         {
                    int zero_flag = 1;
                    int idx = 0;
                    while(yytext[idx]!='\0'){
                        if(yytext[idx]!='x' && yytext[idx]!='X' && yytext[idx]!='0')zero_flag = 0;
                        idx++;
                    }
                    //if the INT is 0
                    if(strcmp(yytext, "0") == 0 
                        || strcmp(yytext, "0x0") == 0
                            || strcmp(yytext, "00") == 0
                                || zero_flag)
                    {
                        yylval.node = newnode("INT", INT, 0, yylineno, 0);
                        return INT;
                    }

                    //tanslate string to int
                    long int ret = 0;
                    char *end;
                    ret = strtol(yytext, &end, 0);
                    if(ret == 0L 
                        || end - yytext < yyleng)
                    {
                        print_lexical_error();
                    }
                    //it's OK
                    yylval.node = newnode("INT", INT, 0, yylineno, ret);
                    return INT;
                }
{r_float}       {
                    int zero_flag = 1;
                    int idx = 0;
                    while(yytext[idx]!='\0'){
                        if(yytext[idx]!='e' 
                            && yytext[idx]!='E' 
                                && yytext[idx]!='.' 
                                    && yytext[idx]!='0')
                                        zero_flag = 0;
                        idx++;
                    }
                    if(strcmp(yytext, "0") == 0
                        || strcmp(yytext, "0.0") == 0
                            || zero_flag)
                    {
                        yylval.node = newnode("FLOAT", FLOAT, 0, yylineno, 0.0);
                        return FLOAT;
                    }
                    double ret = 0;
                    int res;
                    char *end;
                    //ret = strtod(yytext, &end);
                    res = sscanf(yytext,"%lf",&ret);
                    //printf("text: \'%s\'\n", yytext);
                    //printf("ret: %lf\n", ret);
                    if(res<=0)
                    {
                        print_lexical_error();
                    }
                    yylval.node = newnode("FLOAT", FLOAT, 0, yylineno, ret);
                    return FLOAT;
                }
{r_type}        { yylval.node = newnode("TYPE", TYPE, 0, yylineno, yytext);               return TYPE; }
{r_id}          { yylval.node = newnode("ID", ID, 0, yylineno, yytext);                   return ID; }
<<EOF>>         { yyterminate(); }
{r_other}       { print_lexical_error(); }
{r_comment}     {}
{r_err_comment} { print_lexical_error(); }


%%



int print_lexical_error(){
    error_cnt++;
    printf("Error type A at Line %d: Mysterious characters \'%c\'.\n", yylineno, yytext[0]);
}
