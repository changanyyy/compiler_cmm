#ifndef _SYNTAX_H_
#define _SYNTAX_H_





enum GTNode_type{
    EPSILON = -2,
    UNTERMINAL = -1     
};

struct GTNode{
    char name[16];
    int type;
    union {
        long int val_int;
        double val_float;
        char val_string[50];
    } val;

    int first_line;
    
    //产生式标号，在语义分析有用
    int no;

    struct GTNode *children;
    struct GTNode *next;
} *root;



struct GTNode* newnode(char *name, int type, int n, ... );
void print_tree(struct GTNode *r, int base);


int error_cnt;

int print_lexical_error();

void yyerror(const char *);



#endif