#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_


#include"../include/type.h"
#include"../include/syntax.h"
#include"../symtab/symtab.h"


//用来自动生成函数
//Name0代表头为Name的产生式的调度函数
//语法分析树节点的no域记录产生式标号，通过判断语法分析节点标号来分配到相应的Name-no函数
//具体见semantic.c


//生成一个语义分析函数（综合属性）
#define GENF(name,num) \
    SN *name##num(struct GTNode *node)

//生成一个语义分析函数（包含继承属性）
#define GENF2(name,num) \
    SN *name##num(struct GTNode *node, SN* r)



//参数节点，用来表示参数类型
typedef struct ParaTypeNode{
    Type type;
    struct ParaTypeNode *next;
} PTN;

//符号表里面的函数节点
typedef struct FunPara{
    int paranum;
    PTN *typelist;
} FP;

//创建新的参数类型节点
#define NEWPTN(typ) \
    PTN *newptn = (PTN *)malloc(sizeof(PTN)); \
    newptn->type = typ;\
    newptn->next = NULL;


//下面是辅助结构
//里面传递了综合属性或继承属性
typedef struct SemNode{
    //如果是变量
    Type type;    
    
    //备用
    float valfloat;
    int valint;
    struct GTNode* node;  

    int store;

    FieldList fl;

    //传递参数链表
    //分析函数的时候会用到这个field
    PTN *ptn; 

} SN;


#define PERROR(no,line) \
    {int a = no;\
    printf("Error type %d at Line %s", a, line);}

GENF(Program, 0);
GENF(Program, 1);

GENF(ExtDefList, 0);
GENF(ExtDefList, 1);
GENF(ExtDefList, 2);

GENF(ExtDef, 0);
GENF(ExtDef, 1);
GENF(ExtDef, 2);
GENF(ExtDef, 3);


GENF2(ExtDecList, 0);
GENF2(ExtDecList, 1);
GENF2(ExtDecList, 2);



GENF(Specifier, 0);
GENF(Specifier, 1);
GENF(Specifier, 2);


GENF(StructSpecifier, 0);
GENF(StructSpecifier, 1);
GENF(StructSpecifier, 2);


GENF2(OptTag, 0);
GENF2(OptTag, 1);
GENF2(OptTag, 2);

GENF(Tag, 0);
GENF(Tag, 1);


GENF2(VarDec, 0);
GENF2(VarDec, 1);
GENF2(VarDec, 2);


GENF2(FunDec, 0);
GENF2(FunDec, 1);
GENF2(FunDec, 2);


GENF(VarList, 0);
GENF(VarList, 1);
GENF(VarList, 2);

GENF(ParamDec, 0);
GENF(ParamDec, 1);

GENF(CompSt, 0);
GENF(CompSt, 1);

GENF(StmtList, 0);
GENF(StmtList, 1);
GENF(StmtList, 2);


GENF(Stmt, 0);
GENF(Stmt, 1);
GENF(Stmt, 2);
GENF(Stmt, 3);
GENF(Stmt, 4);
GENF(Stmt, 5);
GENF(Stmt, 6);


GENF(DefList, 0);
GENF(DefList, 1);
GENF(DefList, 2);


GENF(Def, 0);
GENF(Def, 1);


GENF2(DecList, 0);
GENF2(DecList, 1);
GENF2(DecList, 2);

GENF2(Dec, 0);
GENF2(Dec, 1);
GENF2(Dec, 2);


GENF(Exp, 0);
GENF(Exp, 1);
GENF(Exp, 2);
GENF(Exp, 3);
GENF(Exp, 4);
GENF(Exp, 5);
GENF(Exp, 6);
GENF(Exp, 7);
GENF(Exp, 8);
GENF(Exp, 9);
GENF(Exp, 10);
GENF(Exp, 11);
GENF(Exp, 12);
GENF(Exp, 13);
GENF(Exp, 14);
GENF(Exp, 15);
GENF(Exp, 16);
GENF(Exp, 17);
GENF(Exp, 18);

GENF(Args, 0);
GENF(Args, 1);
GENF(Args, 2);



#endif