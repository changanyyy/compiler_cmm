#include<stdlib.h>
#include"semantic.h"
#include"../include/syntax.h"
#include"../syntax.tab.h"


//当前处理的函数
STE *cur_func;
bool deal_struct;


void print_error(int number, int lline){
    printf("Error type %d at Line %d\n", number, lline);
    return;
}



//代表整个程序，只有一个产生式
GENF(Program, 0){
    cur_func = NULL;
    deal_struct = false;
    switch (node->no)
    {
    case 1: Program1(node);break;
    default: break;
    }
    return NULL;
}
GENF(Program, 1){
    ExtDefList0(node->children);
    return NULL;
}



//外部定义列表
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
    case 1: ExtDef1(node);break;
    case 2: ExtDef2(node);break;
    case 3: ExtDef3(node);break;
    default: break;
    }
    return NULL;
}
//声明一个变量
GENF(ExtDef, 1){
    //printf("123\n");
    SN *res;
    res = Specifier0(node->children);
    
    //具体类型已经通过res传递
    if(!res)
        return NULL;
    ExtDecList0(node->children->next, res);
    return NULL;
}
//有可能声明一个结构
GENF(ExtDef, 2){
    
    Specifier0(node->children);

    return NULL;
}
//声明一个函数
GENF(ExtDef, 3){
    SN *res1, *res2;
    res1 = Specifier0(node->children);
    FunDec0(node->children->next, res1);
    
    res2 = CompSt0(node->children->next->next);
    cur_func = NULL;
    if(!res1 || !res2)
        return NULL;
    //如果返回值类型和函数返回值不对应
    if(!compare_type(res1->type, res2->type)){
        print_error(8, res2->node->first_line);
        return NULL;
    }
    return NULL;
}


//外部声明列表
GENF2(ExtDecList,0){
    switch (node->no)
    {
    case 1: ExtDecList1(node, r);break;
    case 2: ExtDecList2(node, r);break;
    default: break;
    }
    return NULL;
}
GENF2(ExtDecList,1){
    VarDec0(node->children, r);
    return NULL;
}
GENF2(ExtDecList,2){
    VarDec0(node->children, r);
    ExtDecList0(node->children->next->next, r);
    return NULL;
}


//返回类型，通过res->type表示
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
//返回基础类型
GENF(Specifier, 1){
    Type t = malloc(sizeof(struct Type_));
    t->kind = BASIC;
    //枚举定义在syntax.tab.h里
    if(!strcmp(node->children->val.val_string, "int"))
        t->u.basic = INT;
    else
        t->u.basic = FLOAT;
    
    //把TYPE代表的类型通过res传出去
    SN *res = (SN *)malloc(sizeof(SN));
    res->type = t;
    return res;
}
//返回结构体类型
GENF(Specifier, 2){
    SN *res;
    
    res = StructSpecifier0(node->children);
    //printf("123\n");
    return res;
}


//返回结构体类型
GENF(StructSpecifier, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = StructSpecifier1(node);break;
    case 2: res = StructSpecifier2(node);break;
    default: break;
    }
    return res;
}


//判断结构体是否有重定义的成员
bool redef_member(FieldList fl){
    if(!fl)
        return false;
    FieldList cur = fl;
    FieldList nxt = fl->tail;
    while(nxt != NULL){
        while(nxt != NULL){
            //printf("%s\n", fl->name);
            //printf("%s\n", fl->tail->name);
            if(!strcmp(cur->name, nxt->name)){
                return true;
            }
            nxt = nxt->tail;
        }
        cur = cur->tail;
        nxt = cur->tail;
    }
    return false;

}



//定义一个新的结构体，返回结构体类型
GENF(StructSpecifier, 1){
    //printf("123\n");
    SN *res;
    deal_struct = true;
    res = DefList0(node->children->next->next->next);
    deal_struct = false;
    //printf("1234\n");
    if(res->assigned == 100){
        print_error(15, res->node->first_line);
        return NULL;
    }
    //printf("123\n");
    if(redef_member(res->fl)){
        print_error(15, res->node->first_line);
        return NULL;        
    }
//printf("123\n");
    //下面把结构体成员列表通过res传入OptTag
    res = OptTag0(node->children->next, res);
    //返回的type是结构体
    return res;
}
//使用已经定义过的结构体，返回结构体类型
GENF(StructSpecifier, 2){
    SN *res = Tag0(node->children->next);
    //printf("name %s\n", res->node->val.val_string);
    STE *ste = search_entry(res->node->val.val_string);
    if(ste == NULL){
        //使用了未定义的结构体
        print_error(17, node->children->first_line);
        return NULL;
    }
    res->type = ste->type;
    return res;
}


//一个可选结构体名称，返回的是结构体类型
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
    if(!r)
        return NULL;
    //printf("%s\n", node->children->val.val_string);
    STE *ste = search_entry(node->children->val.val_string);
    if(ste){
        //重复定义结构体
        print_error(16, node->children->first_line);
        return NULL;
    }
    
    
    Type t = malloc(sizeof(struct Type_));
    t->kind = STRUCTURE;
    //通过前面deflist得到的fl链表
    t->u.structure = r->fl;
    //创建表项
    ste = create_entry(false, t, node->children);
    // if(ste){
    //     printf("%s\n", ste->name);
    // }
    //把结构体类型返回
    SN *res = (SN *)malloc(sizeof(SN));
    res->type = t;
    return res;
}
//匿名结构体，不必把这个结构类型存起来，但是要返回这个类型给后面变量用
GENF2(OptTag, 2){
    if(!r)
        return NULL;
    Type t = malloc(sizeof(struct Type_));
    t->kind = STRUCTURE;
    t->u.structure = r->fl;
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
    res->node = node->children;
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
    
    //如果是结构体解析触发这个产生式，那么r->store=100
    //这时不需要把变量存在表里面
    //如果通过外部声明触发的话，需要存在符号表里

    if(r->store!=100 && deal_struct == false){

        STE *ste = search_entry(node->children->val.val_string);
        if(ste){
            //重复定义
            print_error(3,node->children->first_line);
            return NULL;
        }
        create_entry(false, r->type, node->children);
        //printf("%s\n", node->children->val.val_string);
        //ste = search_entry(node->children->val.val_string);
        //printf("%s\n", ste->name);
    }
    SN *res = (SN *)malloc(sizeof(SN));
    //由于结构体需要构造FieldList，需要id名字
    res->type = r->type;
    res->node = node->children;
    return res;
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
    return NULL;
}
GENF2(FunDec, 1){
    //创建符号表项，是函数
    STE *ste = create_entry(true, NULL, node->children);
    //返回值类型
    ste->rettype = r->type;
    
    //获得参数类型列表
    SN *res;
    res = VarList0(node->children->next->next);

    //存函数参数
    ste->paratype = (FP *)malloc(sizeof(FP));
    ste->paratype->paranum = res->valint;
    ste->paratype->typelist = res->ptn;

    cur_func = ste;

    return NULL;
}
GENF2(FunDec, 2){
    //创建符号表项，函数
    STE *ste;
    ste = search_entry(node->children->val.val_string);
    if(ste){
        print_error(4, node->children->first_line);
        return NULL;
    }
    
    ste = create_entry(true, NULL, node->children);
    //返回值类型
    ste->rettype = r->type;
    
    //存函数参数
    ste->paratype = (FP *)malloc(sizeof(FP));
    ste->paratype->paranum = 0;
    ste->paratype->typelist = NULL;

    cur_func = ste;

    return NULL;
}



GENF(VarList, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = VarList1(node);break;
    case 2: res = VarList2(node);break;
    default: break;
    }
    return res;
}
GENF(VarList, 1){
    SN *res1, *res2;
    res1 = VarList0(node->children->next->next);
    
    res2 = ParamDec0(node->children);
    
    NEWPTN(res2->type)
    strcpy(newptn->name, res1->node->val.val_string);

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
    strcpy(newptn->name, res->node->val.val_string);
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
    if(!res)
        return NULL;
    //函数参数列表 不需要存在符号表
    res->store = 100;
    res = VarDec0(node->children->next, res);
    return res;
}



GENF(CompSt, 0){
    //printf("123\n");
    SN *res;
    switch (node->no)
    {
    case 1: res = CompSt1(node);break;
    default: break;
    }
    return res;
}
GENF(CompSt, 1){
    DefList0(node->children->next);
    
    SN *res;
    res = StmtList0(node->children->next->next);
    return res;
}



GENF(StmtList, 0){

    SN *res;
    switch (node->no)
    {
    case 1: res = StmtList1(node);break;
    case 2: res = StmtList2(node);break;
    default: break;
    }
    return res;
}
GENF(StmtList, 1){
    SN *res1, *res2;
    res1 = Stmt0(node->children);

    res2 = StmtList0(node->children->next);
    //返回包含return语句的返回值
    if(!res2){
        return res1;
    }
    return res2;
}
GENF(StmtList, 2){
    return NULL;
}



GENF(Stmt, 0){
    SN *res = NULL;

    switch (node->no)
    {
    case 1: Stmt1(node);break;
    case 2: Stmt2(node);break;
    case 3: res = Stmt3(node);break;
    case 4: Stmt4(node);break;
    case 5: Stmt5(node);break;
    case 6: Stmt6(node);break;
    default: break;
    }
    return res;
}
GENF(Stmt, 1){
    
    Exp0(node->children);
    return NULL;
}
GENF(Stmt, 2){
    CompSt0(node->children);
    return NULL;
}
GENF(Stmt, 3){
    SN *res;
    res = Exp0(node->children->next);
    return res;
}
GENF(Stmt, 4){
    Exp0(node->children->next->next);
    Stmt0(node->children->next->next->next->next);
    return NULL;
}
GENF(Stmt, 5){
    Exp0(node->children->next->next);
    Stmt0(node->children->next->next->next->next);
    Stmt0(node->children->next->next->next->next->next->next);
    return NULL;
}
GENF(Stmt, 6){
    Exp0(node->children->next->next);
    Stmt0(node->children->next->next->next->next);
    return NULL;
}



GENF(DefList, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = DefList1(node);break;
    case 2: res = DefList2(node);break;
    default: break;
    }
    return res;
}
GENF(DefList, 1){
    SN *res1,*res2;
    res1 = Def0(node->children);
    res2 = DefList0(node->children->next);

    if(!res1){
        return NULL;
    }
    //TODO
    if(res2 && res2->fl){
        //如果下一个定义列表不是空，那就接到def的fl末尾
        FieldList fl = res1->fl;
        while(fl->tail){
            fl = fl->tail;
        }
        
        fl->tail = res2->fl;
    }
    // if(res1->fl && res1->fl->tail){
    //     printf("123\n");
    // }

    if(res1->assigned == 100 ||(res2 && res2->assigned == 100))
    {
        res1->assigned == 100;
    }
    //直接返回接好的链表头
    return res1;
}
GENF(DefList, 2){
    return NULL;
}



GENF(Def, 0){
    SN* res;
    switch (node->no)
    {
    case 1: res = Def1(node);break;
    default: break;
    }
    return res;
}
GENF(Def, 1){
    SN *res;
    res = Specifier0(node->children);
    if(!res)
        return NULL;
    res = DecList0(node->children->next, res);
    if(!res)
        return NULL;
    FieldList fl=res->fl;
    while(fl){
        //printf("aa= %s\n",fl->name);
        fl=fl->tail;
    }
    //传回declist的fl链表
    //printf("123\n");
    return res;
}



GENF2(DecList, 0){
    
    SN *res;
    switch (node->no)
    {
    case 1: res = DecList1(node, r); break;
    case 2: res = DecList2(node, r); break;
    default: break;
    }
    return res;
}
GENF2(DecList, 1){
    SN *res;
    //获取Dec的类型
    res = Dec0(node->children, r);

    if(!res)
        return NULL;

    FieldList fl = malloc(sizeof(struct FieldList_));

    strcpy(fl->name,res->node->val.val_string);
    
    fl->type = res->type;
    fl->tail = NULL;

//printf("kk = %s\n",res->node->val.val_string);
    //把FieldList传回去
    res->fl = fl;
    return res;

}
GENF2(DecList, 2){
    SN *res1, *res2;
    //获取Dec的类型和名字
    res1 = Dec0(node->children, r);

    //获得declist的fl链表，传入r里面有类型
    res2 = DecList0(node->children->next->next, r);

//printf("kk = %s\n",res1->node->val.val_string);
    if((res1==NULL) || (res2==NULL))
        return NULL;
    //printf("kk = %s\n",res2->node->val.val_string);
    FieldList fl = malloc(sizeof(struct FieldList_));
    strcpy(fl->name,res1->node->val.val_string);
    //printf("kk = %s\n",res1->node->val.val_string);
    fl->type = res1->type;

    fl->tail = res2->fl;

    if(res1->assigned == 100
        || res2->assigned== 100){
        res2->assigned = 100;
    }

    //把构造好的新fl列表传回去
    res2->fl = fl;
    return res2;
}




GENF2(Dec, 0){
    SN *res;
    switch (node->no)
    {
    case 1: res = Dec1(node, r);break;
    case 2: res = Dec2(node, r);break;
    default: break;
    }
    return res;
}
GENF2(Dec, 1){
    SN *res1;
    //r->store = 100;
    res1 = VarDec0(node->children, r);
    //if(res1)
    //    printf("wda %s\n", res1->node->val.val_string);
    return res1;
}

GENF2(Dec, 2){
    SN *res1, *res2;

    //r->store = 100;

    res1 = VarDec0(node->children, r);

    res2 = Exp0(node->children->next->next);

    if(!res2)
        return NULL;

    if(!compare_type(res1->type, res2->type)){
        //赋值号两边类型不同
        print_error(5, node->children->first_line);
        return NULL;
    }

    res1->assigned = 100;
    return res1;
}



GENF(Exp, 0){
    SN *res;
    switch (node->no)
    {
    case 1:  res = Exp1(node);break;
    case 2:  res = Exp2(node);break;
    case 3:  res = Exp3(node);break;
    case 4:  res = Exp4(node);break;
    case 5:  res = Exp5(node);break;
    case 6:  res = Exp6(node);break;
    case 7:  res = Exp7(node);break;
    case 8:  res = Exp8(node);break;
    case 9:  res = Exp9(node);break;
    case 10: res = Exp10(node);break;
    case 11: res = Exp11(node);break;
    case 12: res = Exp12(node);break;
    case 13: res = Exp13(node);break;
    case 14: res = Exp14(node);break;
    case 15: res = Exp15(node);break;
    case 16: res = Exp16(node);break;
    case 17: res = Exp17(node);break;
    case 18: res = Exp18(node);break;
    default: break;
    }
    return res;
}
GENF(Exp, 1){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2){
        return NULL;
    }
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(5, node->children->first_line);
        return NULL;
    }
    //printf("123\n");
    if(node->children->children->type == INT 
        || node->children->children->type == FLOAT)   
    {
        //错误6，赋值号左边出现一个只有右值的表达式
        print_error(6, node->children->first_line);
        return NULL;
    }


    return res1;
}
GENF(Exp, 2){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;

    if(res1->type->kind!=BASIC || res1->type->u.basic!=INT
        || res2->type->kind!=BASIC || res2->type->u.basic!=INT)
    {
        //类型不匹配，只允许两个int进行逻辑运算
        print_error(7, node->children->first_line);        
        return NULL;
    }
    
    return res1;
}
GENF(Exp, 3){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res1->type->u.basic!=INT
        || res2->type->kind!=BASIC || res2->type->u.basic!=INT)
    {
        //类型不匹配，只允许两个int进行逻辑运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    return res1;
}
GENF(Exp, 4){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res2->type->kind!=BASIC){
        //不是基本类型，不能关系运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(7, node->children->first_line);
        return NULL;
    }
    return res1;
}
GENF(Exp, 5){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res2->type->kind!=BASIC){
        //不是基本类型，不能关系运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(7, node->children->first_line);
        return NULL;
    }
    return res1;
}
GENF(Exp, 6){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res2->type->kind!=BASIC){
        //不是基本类型，不能关系运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(7, node->children->first_line);
        return NULL;
    }
    return res1;
}
GENF(Exp, 7){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res2->type->kind!=BASIC){
        //不是基本类型，不能关系运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(7, node->children->first_line);
        return NULL;
    }
    return res1;
}
GENF(Exp, 8){
    SN *res1, *res2;
    res1 = Exp0(node->children);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res1->type->kind!=BASIC || res2->type->kind!=BASIC){
        //不是基本类型，不能关系运算
        print_error(7, node->children->first_line);
        return NULL;
    }
    
    bool res;
    res = compare_type(res1->type, res2->type);
    if(!res){
        //赋值左右两边类型不匹配
        print_error(7, node->children->first_line);
        return NULL;
    }
    return res1;    
}
GENF(Exp, 9){
    SN *res;
    res = Exp0(node->children->next);
    if(!res)
        return NULL;
    return res;
}
GENF(Exp, 10){
    SN *res;
    res = Exp0(node->children->next);
    if(!res)
        return NULL;
    if(res->type->kind != BASIC){

    }

    return res;
}
GENF(Exp, 11){
    SN *res;
    res = Exp0(node->children->next);
    if(!res)
        return NULL;
    if(res->type->kind != BASIC || res->type->u.basic != INT){

    }

    return res;
}


//比较参数是否相同，相同就返回true，不同返回false
bool compare_parameter(PTN *lhs, PTN *rhs){
    if(!lhs && !rhs)
        return true;
    if((lhs && !rhs) || (!lhs && rhs))
        return false;
    //printf("123\n");
    if(!compare_type(lhs->type, rhs->type))
        return false;
        //printf("123\n");
    //递归检查
    return compare_parameter(lhs->next,rhs->next);
}


GENF(Exp, 12){
    STE * ste = search_entry(node->children->val.val_string);
    if(!ste){
        //使用了未定义的函数
        print_error(2, node->children->first_line);
        return NULL;
    }
    if(ste->entrytype != FUNCTION){
        //调用了不是函数
        print_error(11, node->children->first_line);
        return NULL;
    }

    SN *res1 = Args0(node->children->next->next);
    
    // PTN *pt = res1->ptn;
    // while(pt){
    //     printf("11\n");
    //     pt = pt->next;
    // }
    
    
    if(!compare_parameter(ste->paratype->typelist, res1->ptn)){
        //参数不匹配
        print_error(9, node->children->first_line);
        return NULL;        
    }
    //printf("123\n");
    SN* res = (SN *)malloc(sizeof(SN));
    res->type = ste->rettype;
    
    return res;    

}
GENF(Exp, 13){
    STE *ste = search_entry(node->children->val.val_string);
    if(!ste){
        //使用了未定义的函数
        print_error(2, node->children->first_line);
        return NULL;
    }
    if(ste->entrytype != FUNCTION){
        //不是函数
        print_error(11, node->children->first_line);
        return NULL;
    }
    if(ste->paratype->typelist != NULL){
        //参数不匹配
        print_error(9, node->children->first_line);
        return NULL; 
    }

    return NULL;
}
//访问数组
GENF(Exp, 14){
    SN *res1 = Exp0(node->children);
    SN *res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    if(res2->type->kind != BASIC || res2->type->u.basic != INT){
        //不一样
        print_error(12, node->children->first_line);
        return NULL;
    }
    res2->type = res2->type->u.array.elem;
    if(res1->type->kind!=ARRAY){
        //不是数组
        print_error(10, node->children->first_line);
        return NULL; 

    }

    return res2;
}


FieldList search_member(FieldList fl, const char *name){
    if(!fl)
        return NULL;
    if(!strcmp(fl->name, name))
        return fl;
    else    
        return search_member(fl->tail, name);
}


//访问结构体
GENF(Exp, 15){
    
    SN *res = Exp0(node->children);
    
    if(!res)
        return NULL;
        
    if(res->type->kind != STRUCTURE){
        print_error(13, node->children->first_line);
        return NULL; 
    }
    
    FieldList res1 = search_member(res->type->u.structure, node->children->next->next->val.val_string);
    if(!res1){
        //没有这个成员
        print_error(14, node->children->first_line);
        return NULL; 
    }
    res = malloc(sizeof(SN));
    res -> type = res1->type;
    return res;

}
GENF(Exp, 16){

    STE *ste = search_entry(node->children->val.val_string);

    bool ispara = false;
    PTN *ptn;
    if(cur_func){
        ptn = cur_func->paratype->typelist;
        while(ptn != NULL){
            //printf("%s\n", ptn->name);
            if(!strcmp(ptn->name, node->children->val.val_string)){
                ispara = true;
                break;
            }
            ptn = ptn->next;
        }
    }
    if(!ste && !ispara){
        //printf("%s\n",node->children->val.val_string);
        print_error(1,node->children->first_line);
        return NULL;
    }
    //printf("123\n");

    SN *res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    //在符号表里
    if(ste){
        res->type->kind = ste->type->kind;
        res->type->u = ste->type->u;
        res->valint = 0;
    }
    else{
        res->type->kind = ptn->type->kind;
        res->type->u = ptn->type->u;
        res->valint = 0;
    }
    res->node = node->children;
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




//函数调用参数列表
GENF(Args, 0){
    SN *res;
    switch(node->no){
    case 1: res = Args1(node); break;
    case 2: res = Args2(node); break;
    default: break;
    }

    return res;
}
GENF(Args, 1){
    //获得Exp的类型
    SN *res1 = Exp0(node->children);
    //获得预期参数列表
    SN *res2 = Args0(node->children->next->next);
    // if(res2->ptn){
    //     printf("123\n");
    // }

    //在链表头接入类型
    NEWPTN(res1->type);
    newptn->next = res2->ptn;
    //重复利用res2进行返回
    res2->ptn = newptn;
    return res2;
}
GENF(Args, 2){
    //参数链表尾
    SN *res;
    res = Exp0(node->children);
//printf("123\n");
    NEWPTN(res->type);
    res->ptn = newptn;

    return res;
}