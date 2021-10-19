#include<stdlib.h>
#include"semantic.h"
#include"../include/syntax.h"
#include"../syntax.tab.h"


//分配函数
GENF(Program, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = Program1(node);break;
    default: break;
    }
    return res;
}
//具体处理函数
GENF(Program, 1){
    ExtDefList0(node->children);
    return NULL;
}

GENF(ExtDefList, 0){
    switch (node->no)
    {
    case 1: ExtDefList1(node);break;
    case 2: ExtDefList2(node);break;
    default: break;
    }
    return NULL;
}
GENF(ExtDefList, 1){
    ExtDef0(node->children);
    ExtDefList0(node->children->next);
    return NULL;
}
GENF(ExtDefList, 2){
    return NULL;    
}


GENF(ExtDef, 0){
    switch(node->no)
    {
    case 1: Extdef1(node);break;
    case 2: ExtDef2(node);break;
    case 3: ExtDef3(node);break;
    default: break;
    }
    return NULL;
}
//声明一个变量
GENF(ExtDef, 1){
    SN *res1;
    res1 = Specifier0(node->children);
    ExtDecList0(node->children->next, res1);
    return NULL;
}
//有可能声明一个结构
GENF(ExtDef, 2){
    Specifier0(node);
    return NULL;
}
//--------------------
//声明一个函数
GENF(ExtDef, 3){

}



GENF2(ExtDecList,0){
    switch (node->no)
    {
    case 1: ExtDecList1(node, r);break;
    case 2: ExtDecList2(node, r);break;
    default: break;
    }
}
GENF2(ExtDecList,1){
    VarDec0(node->children, r);
    return NULL;
}
GENF2(ExtDecList,2){
    VarDec0(node->children, r);
    ExtDecList0(node->children->next, r);
    return NULL;
}



GENF(Specifier, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = Specifier1(node);break;
    case 2: res = Specifier2(node);break;
    default: break;
    }    
    return res;
}
GENF(Specifier, 1){
    Type t = malloc(sizeof(struct Type_));
    t->kind = BASIC;
    //枚举定义在syntax.tab.h里
    t->u.basic = node->children->type;
    SN *res = (SN *)malloc(sizeof(SN));
    res->type = t;
    return res;
}
GENF(Specifier, 2){
    SN *res;
    res = StructSpecifier0(node->children);
    return res;
}



GENF(StructSpecifier, 0){
    switch (node->no)
    {
    case 1: StructSpecifier1(node);break;
    case 2: StructSpecifier2(node);break;
    default: break;
    }
}
GENF(StructSpecifier, 1){
    SN *res;
    res = DefList0(node);
    res = OptTag0(node->children->next, res);
    return res;
}
GENF(StructSpecifier, 2){
    SN *res = Tag0(node->children->next);
    STE *ste = search_entry(res->node->name);
    if(ste == NULL){
        //使用了未定义的结构体
    }
    else{
        res->type = ste->type;
    }
    return res;
}



GENF2(OptTag, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = OptTag1(node, r);break;
    case 2: res = OptTag2(node, r);break;
    default: break;
    }
    return res;
}
//在这里我把结构体直接当成一个符号表的表项来处理，也就是说变量不可以重名
GENF2(OptTag, 1){
    Type t = malloc(sizeof(struct Type_));
    t->kind = STRUCT;
    t->u.structure = r->type;
    create_entry(false, t, node->children);
    SN *res = (SN *)malloc(sizeof(SN));
    res->type = t;
    return res;
}
//匿名结构体，不必把这个结构类型存起来，但是要返回这个类型给后面变量用
GENF2(OptTag, 2){
    Type t = malloc(sizeof(struct Type_));
    t->kind = STRUCT;
    t->u.structure = r->type;
    SN *res = (SN *)malloc(sizeof(SN));
    res->type = t;
    return res;
}



GENF(Tag, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = Tag1(node);break;
    default: break;
    }
    return res;
}
//直接返回node
GENF(Tag, 1){
    SN *res = (SN *)malloc(sizeof(SN));
    res->node = node;
    return res;
}



GENF2(VarDec, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = VarDec1(node, r);break;
    case 2: res = VarDec2(node, r);break;
    default: break;
    }
    return res;
}
GENF2(VarDec, 1){
    //类型从右往左一层层构造，通过r来传递
    //遇到ID说明结束传递，可以创建符号表项
    create_entry(false, r->type, node->children);
    return r;
}
//当是一个数组的话
GENF2(VarDec, 2){
    //维数等于产生式中的int值
    int dim = node->children->next->next->val.val_int;
    //创建新Type节点
    Type t = (Type)malloc(sizeof(struct Type_));
    //构造新Type
    t->kind = ARRAY;
    t->u.array.elem = r->type;
    t->u.array.size = dim;

    //将构造出的新type传入产生式的vardec
    SN *res = malloc(sizeof(SN));
    res->type = t;
    res = VarDec0(node->children, res);
    return res;
}



GENF2(FunDec, 0){
    switch (node->no)
    {
    case 1: FunDec1(node, r);break;
    case 2: FunDec2(node, r);break;
    default: break;
    }
}
GENF2(FunDec, 1){
    STE *ste = create_entry(true, NULL, node->children);
    ste->rettype = r->type;
    //TODO
    SN *res;
    res = VarList0(node->children->next->next);

    ste->paratype = malloc(sizeof(FP));
    ste->paratype->paranum = res->valint;
    ste->paratype->typelist = res->ptn;
}
GENF2(FunDec, 2){
    STE *ste = create_entry(true, NULL, node->children);
    ste->rettype = r->type;
    ste->paratype = (FP *)malloc(sizeof(FP));
    ste->paratype->paranum = 0;
    ste->paratype->typelist = NULL;
    return NULL;
}



GENF(VarList, 0){
    switch (node->no)
    {
    case 1: VarList1(node);break;
    case 2: VarList2(node);break;
    default: break;
    }
}
GENF(VarList, 1){
    SN *res1, *res2;
    res1 = VarList0(node->children->next->next);
    
    res2 = ParamDec0(node->children);
    
    NEWPTN(res2->type)

    newptn->next = res1->ptn;

    SN *res = malloc(sizeof(SN));
    res->ptn = newptn;
    res->valint = res1->valint + 1;
    return res;
}
GENF(VarList, 2){
    //这里的ParamDec是最后一个，直接返回参数node即可
    SN *res;
    res = ParamDec0(node->children);

    NEWPTN(res->type);
    res = malloc(sizeof(SN));
    res->ptn = newptn;
    res->valint=1;
    return res;
}



GENF(ParamDec, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = ParamDec1(node);break;
    default: break;
    }
    return res;
}
GENF(ParamDec, 1){
    SN *res;
    res = Specifier0(node->children);
    res = VarDec0(node->children->next, res);
    return res;
}

//-----------------------------

GENF(CompSt, 0){
    switch (node->no)
    {
    case 1: CompSt1(node);break;
    default: break;
    }
}
GENF(CompSt, 1){}



GENF(StmtList, 0){
    switch (node->no)
    {
    case 1: StmtList1(node);break;
    default: break;
    }
}
GENF(StmtList, 1){}



GENF(Stmt, 0){
    switch (node->no)
    {
    case 1: Stmt1(node);break;
    default: break;
    }
}
GENF(Stmt, 1){}
GENF(Stmt, 2){}
GENF(Stmt, 3){}
GENF(Stmt, 4){}
GENF(Stmt, 5){}
GENF(Stmt, 6){}



GENF(DefList, 0){
    switch (node->no)
    {
    case 1: DefList1(node);break;
    case 2: DefList2(node);break;
    default: break;
    }
}
GENF(DefList, 1){}
GENF(DefList, 2){}



GENF(Def, 0){
    switch (node->no)
    {
    case 1: Def1(node);break;
    default: break;
    }
}
GENF(Def, 1){}



GENF(DecList, 0){
    switch (node->no)
    {
    case 1: DecList1(node);break;
    case 2: DecList2(node);break;
    default: break;
    }
}
GENF(DecList, 1){}
GENF(DecList, 2){}



GENF(Dec, 0){
    switch (node->no)
    {
    case 1: Dec1(node);break;
    case 2: Dec2(node);break;
    default: break;
    }
}
GENF(Dec, 1){}
GENF(Dec, 2){}



GENF(Exp, 0){
    SN *res;
    switch (node->no)
    {
    case 1:  Exp1(node);break;
    case 2:  Exp2(node);break;
    case 3:  Exp3(node);break;
    case 4:  Exp4(node);break;
    case 5:  Exp5(node);break;
    case 6:  Exp6(node);break;
    case 7:  Exp7(node);break;
    case 8:  Exp8(node);break;
    case 9:  Exp9(node);break;
    case 10: Exp10(node);break;
    case 11: Exp11(node);break;
    case 12: Exp12(node);break;
    case 13: Exp13(node);break;
    case 14: Exp14(node);break;
    case 15: Exp15(node);break;
    case 16: Exp16(node);break;
    case 17: Exp17(node);break;
    case 18: Exp18(node);break;
    default: break;
    }
    return res;
}
GENF(Exp, 1){}
GENF(Exp, 2){}
GENF(Exp, 3){}
GENF(Exp, 4){}
GENF(Exp, 5){}
GENF(Exp, 6){}
GENF(Exp, 7){}
GENF(Exp, 8){}
GENF(Exp, 9){}
GENF(Exp, 10){}
GENF(Exp, 11){}
GENF(Exp, 12){}
GENF(Exp, 13){}
GENF(Exp, 14){}
GENF(Exp, 15){}
GENF(Exp, 16){
    STE *ste = search_entry(node->children->name);
    if(!ste)
        PERROR(1,node->children->first_line)
    SN *res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    res->type->kind = ste->type->kind;
    res->type->u = ste->type->u;
    return res;
}
GENF(Exp, 17){
    SN* res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    res->type->kind = BASIC;
    res->type->u.basic = INT;
    res->node = node->children;
    return res;
}
GENF(Exp, 18){
    SN *res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    res->type->kind = BASIC;
    res->type->u.basic = FLOAT;
    res->node = node->children;
    return res;
}

GENF(Args, 0){}
GENF(Args, 1){}
