#ifndef _SYNTAX_H_
#define _SYNTAX_H_

/*
* 语法相关的结构
* 
*/



//在bison中使用，其中终结符是非负数，
//epsilon和非终结符用负数表示
enum GTNode_type{
    EPSILON = -2,
    UNTERMINAL = -1     
};


//语法分析树节点
struct GTNode{
    //终结符或者非终结符名字
    char name[16];
    //type，和上面对应
    int type;
    //值，对于int,float,id有不同类型
    //所以用union
    union {
        long int val_int;
        double val_float;
        char val_string[50];
    } val;

    //对应行号
    int first_line;
    
    //产生式标号，在语义分析有用
    int no;

    //用二叉树形式表示语法分析树，
    //children是子节点链表
    //next是子节点链表的下一个兄弟
    struct GTNode *children;
    struct GTNode *next;
} *root;


//创建一个新的节点
//定义syntax.y
struct GTNode* newnode(char *name, int type, int n, ... );
void print_tree(struct GTNode *r, int base);

//错误计数
int error_cnt;

//打印错误
int print_lexical_error();
void yyerror(const char *);



#endif