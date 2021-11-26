#include<stdlib.h>
#include"semantic.h"
#include"../include/syntax.h"
#include"../syntax.tab.h"


//当前处理的函数
STE *cur_func;

//在处理结构体的时候使用，这样就不用把变量名存入符号表。
bool deal_struct;

//参数解析时候的错误，不是参数不匹配
bool arguments_fault;

//错误buffer，给结构体定义里错误用的
struct ErrorBuffer head;

bool deal_fun_dec;
bool deal_same_fun;

bool compare_parameter(PTN *lhs, PTN *rhs, int i, char * s);


void print_error(int number, int lline){
    printf("Error type %d at Line %d\n", number, lline);
    return;
}

//创建新的error节点
struct ErrorBuffer *newEB(char *errstr, int lineno){
    struct ErrorBuffer* eb = malloc(sizeof(struct ErrorBuffer));
    strcpy(eb->errstr, errstr);
    eb->lineno = lineno;
    eb->next = NULL;
    return eb;
}

//把错误信息插入buffer
void insert_error_buffer(char *errstr, int lineno){
    struct ErrorBuffer * eb = newEB(errstr, lineno);

    struct ErrorBuffer *cur = &head, *nxt = (&head)->next;
    bool insert = false;
    while(nxt){
        if(cur->lineno <= eb->lineno && eb->lineno < nxt->lineno){
            cur->next = eb;
            eb->next = nxt;
            insert = true;
            break;
        }
        cur= cur->next;
        nxt = cur->next;
    }
    if(!insert){
        cur->next = eb;
    }
    return;
}

//打印错误信息并清空buffer
void print_error_buffer(){
    struct ErrorBuffer *cur = head.next;
    struct ErrorBuffer *nxt;
    while(cur){
        nxt = cur->next;
        printf("%s", cur->errstr);
        free(cur);
        cur = nxt;
    }
    head.next = NULL;
    return;
}


//代表整个程序，只有一个产生式
GENF(Program, 0){
    //下面进行一些初始化操作
    cur_func = NULL;
    deal_struct = false;
    arguments_fault = false;
    head.lineno = -1;
    head.next = NULL;
    deal_fun_dec = false;
    deal_same_fun = false;


    //添加read函数
    struct GTNode *nn1 = malloc(sizeof(struct GTNode));
    strcpy(nn1->val.val_string, "read");
    STE * ste1 = create_entry(true, NULL, nn1);

    Type t1 = malloc(sizeof(struct Type_));
    t1->kind = BASIC;
    t1->u.basic = INT;
    ste1->rettype = t1;
    ste1->isdefine = true;
    ste1->paratype = malloc(sizeof(struct ParaTypeNode));
    ste1->paratype->paranum = 0;
    ste1->paratype->typelist = NULL;
    insert_entry(ste1);//TODO


    //添加write函数
    struct GTNode *nn2 = malloc(sizeof(struct GTNode));
    strcpy(nn2->val.val_string, "write");
    STE * ste2 = create_entry(true, NULL, nn2);
    ste2->rettype =NULL;
    ste2->isdefine = true;
    Type t2 = malloc(sizeof(struct Type_));
    t2->kind = BASIC;
    t2->u.basic = INT;
    NEWPTN(t2);
    ste2->paratype = malloc(sizeof(struct ParaTypeNode));
    ste2->paratype->paranum = 1;
    ste2->paratype->typelist = newptn;
    insert_entry(ste2);
    
    switch (node->no)
    {
    case 1: Program1(node);break;
    default: break;
    }
    scan_fun();

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
    case 4: ExtDef4(node);break;
    default: break;
    }
    return NULL;
}

//声明一个变量
GENF(ExtDef, 1){
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

//定义一个函数
GENF(ExtDef, 3){
    SN *res1;
    res1 = Specifier0(node->children);
    FunDec0(node->children->next, res1);
    deal_same_fun = false;
    CompSt0(node->children->next->next);
    cur_func = NULL;
    return NULL;
}

//声明一个函数
GENF(ExtDef, 4){
    //printf("11\n");
    SN *res1;
    res1 = Specifier0(node->children);
    bool tmp_deal_fun_dec = deal_fun_dec;
    deal_fun_dec = true;
    FunDec0(node->children->next, res1);
    deal_fun_dec = tmp_deal_fun_dec;
    cur_func = NULL;
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


//类型产生式，返回类型，通过res->type表示
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
//重复就返回重复的行号，否则0
int redef_member(FieldList fl){
    if(!fl)
        return 0;
    FieldList cur = fl;
    FieldList nxt = fl->tail;
    while(nxt != NULL){
        while(cur != nxt){
            if(!strcmp(cur->name, nxt->name)){
                char ss[50];
                sprintf(ss,"Error type 15 at Line %d\n", nxt->lline);
                insert_error_buffer(ss, nxt->lline);
                break;
            }
            cur = cur->tail;
        }
        cur = fl;
        nxt = nxt->tail;
    }
    return 0;

}



//定义一个新的结构体，返回结构体类型
GENF(StructSpecifier, 1){
    bool tmp_deal_struct = deal_struct;
    deal_struct = true;
    SN *res;
    res = DefList0(node->children->next->next->next);
    deal_struct = tmp_deal_struct;

    if(!res){
        res = malloc(sizeof(SN));
        res->fl = NULL;
        OptTag0(node->children->next,res);
        return NULL;
    }

    int redefline = redef_member(res->fl);
    print_error_buffer();
    
    
    //下面把结构体成员列表通过res传入OptTag
    res = OptTag0(node->children->next, res);

    //返回的type是结构体
    return res;
}
//使用已经定义过的结构体，返回结构体类型
GENF(StructSpecifier, 2){
    SN *res = Tag0(node->children->next);
    STE *ste = search_entry(res->node->val.val_string);
    
    if(ste == NULL){
        //使用了未定义的结构体
        print_error(17, node->children->next->first_line);
        return NULL;
    }else{
        res->type = ste->type;
    }
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
    
    //如果deal_struct是true（处理结构体内部）
    //这时不需要把变量存在表里面
    //如果通过外部声明触发的话，需要存在符号表里
    STE *ste = search_entry(node->children->val.val_string);
   
    if(ste && !deal_same_fun){
        //重复定义
        print_error(3,node->children->first_line);
        //ste->type = r->type;
        return NULL;
    }
    else if(ste && deal_same_fun){
        ste->type = r->type;
        SN *res = (SN *)malloc(sizeof(SN));
        //由于结构体需要构造FieldList，需要id名字
        res->type = r->type;
        res->node = node->children;
        return res;
    }
    

    if(deal_struct == false){
        //printf("%d %s\n",node->children->first_line, node->children->val.val_string);
        create_entry(false, r->type, node->children);
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

    if(r->type->kind == BASIC){
        t->u.array.width = 4;
    }
    else if(r->type->kind == ARRAY){
        t->u.array.width = r->type->u.array.size * r->type->u.array.width;
    }

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
    //printf("11\n");
    STE *ste = search_entry(node->children->val.val_string);
    //获得参数类型列表
    bool eq1, eq2;
    SN *res;
    if(ste){
        deal_same_fun = true;
    }
    res = VarList0(node->children->next->next);
    deal_same_fun = false;
    if(!res){
        return NULL;
    }
    if(deal_fun_dec){//处理函数声明

        if(!ste){//没有表项就创建
            ste = create_entry(true, NULL, node->children);
            ste->lline = node->children->first_line;
            ste->rettype = r->type;
            ste->isdefine = false;
            //存函数参数
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = res->valint;
            ste->paratype->typelist = res->ptn;
            cur_func = ste;
            return NULL;
            res = malloc(sizeof(SN));
            return res;
        }
        else{//已经有表项，要比较是否一致
            //不一致就报错
            //printf("111");
            eq1 = compare_type(ste->rettype, r->type);
            eq2 = compare_parameter(ste->paratype->typelist, res->ptn, 0, "");
            if(!eq1 || !eq2){
                print_error(19, node->children->first_line);
                return NULL;
            }
            return NULL;
        }
    }
    else{
        //处理函数定义
        if(ste && ste->isdefine == false){
            eq1 = compare_type(ste->rettype, r->type);
            eq2 = compare_parameter(ste->paratype->typelist, res->ptn, 0, "");
            if(!eq1 || !eq2){
                print_error(19, node->children->first_line);
                //return NULL;
            }
            ste->isdefine = true;
            ste->rettype = r->type;
            ste->paratype->paranum = res->valint;
            //printf("para num = %d\n", res->valint);
            ste->paratype->typelist = res->ptn;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }
        else if(ste && ste->isdefine == true){
            print_error(4, node->children->first_line);
            ste->rettype = r->type;
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = res->valint;
            ste->paratype->typelist = res->ptn;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }
        else{
            //创建符号表项，是函数
            ste = create_entry(true, NULL, node->children);
            //返回值类型
            ste->lline = node->children->first_line;
            ste->isdefine = true;
            ste->rettype = r->type;
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = res->valint;
            ste->paratype->typelist = res->ptn;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }

    }
    
    
    return res;
}
GENF2(FunDec, 2){
    //创建符号表项，函数
    STE *ste;
    ste = search_entry(node->children->val.val_string);
    deal_same_fun = false;
    //获得参数类型列表
    bool eq1, eq2;
    SN *res;
    if(deal_fun_dec){//处理函数声明
        if(!ste){//没有表项就创建
            ste = create_entry(true, NULL, node->children);
            ste->rettype = r->type;
            ste->isdefine = false;
            //存函数参数
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = 0;
            ste->paratype->typelist = NULL;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }
        else{//已经有表项，要比较是否一致
            //不一致就报错
            eq1 = compare_type(ste->rettype, r->type);
            eq2 = compare_parameter(ste->paratype->typelist, NULL, 0, "");
            if(!eq1 || !eq2){
                print_error(9, node->children->first_line);
                return NULL;
            }
            return NULL;
        }
    }
    else{
        //处理函数定义
        if(ste && ste->isdefine == false){
            eq1 = compare_type(ste->rettype, r->type);
            eq2 = compare_parameter(ste->paratype->typelist, NULL, 0, "");
            if(!eq1 || !eq2){
                print_error(9, node->children->first_line);
                return NULL;
            }
            ste->isdefine = true;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }
        else if(ste && ste->isdefine == true){
            print_error(4, node->children->first_line);
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = 0;
            ste->paratype->typelist = NULL;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }
        else{
            //创建符号表项，是函数
            ste = create_entry(true, NULL, node->children);
            //返回值类型
            ste->isdefine = true;
            ste->rettype = r->type;
            ste->paratype = (FP *)malloc(sizeof(FP));
            ste->paratype->paranum = 0;
            ste->paratype->typelist = NULL;
            cur_func = ste;
            res = malloc(sizeof(SN));
            return res;
        }

    }
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
    
    res1 = ParamDec0(node->children);
    res2 = VarList0(node->children->next->next);
    
    
    if(!res1 && !res2)
        return NULL;

    SN *res = malloc(sizeof(SN));

    if(res1 && !res2){
        NEWPTN(res1->type)
        strcpy(newptn->name, res1->node->val.val_string);
        res->ptn = newptn;
        newptn->next = NULL;
        res->valint = 0;
        return res;
    }
    else if(!res1 && res2){
        return res2;
    }
    else{
        NEWPTN(res1->type)
        strcpy(newptn->name, res1->node->val.val_string);
        res->ptn = newptn;
        newptn->next = res2->ptn;
        res->valint = res2->valint+1;
        return res;
    }
    
    res->valint = res2->valint + 1;
    return res;
}
GENF(VarList, 2){
    //这里的ParamDec是最后一个，直接返回参数node即可
    SN *res;
    res = ParamDec0(node->children);
    if(!res)    
        return NULL;
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
    res = VarDec0(node->children->next, res);
    return res;
}



GENF(CompSt, 0){
    switch (node->no)
    {
    case 1: CompSt1(node);break;
    default: break;
    }
    return NULL;
}
GENF(CompSt, 1){
    DefList0(node->children->next);
    StmtList0(node->children->next->next);
    return NULL;
}



GENF(StmtList, 0){
    switch (node->no)
    {
    case 1: StmtList1(node);break;
    case 2: StmtList2(node);break;
    default: break;
    }
    return NULL;
}
GENF(StmtList, 1){
    Stmt0(node->children);
    StmtList0(node->children->next);
    return NULL;
}
GENF(StmtList, 2){
    return NULL;
}



GENF(Stmt, 0){
    switch (node->no)
    {
    case 1: Stmt1(node);break;
    case 2: Stmt2(node);break;
    case 3: Stmt3(node);break;
    case 4: Stmt4(node);break;
    case 5: Stmt5(node);break;
    case 6: Stmt6(node);break;
    default: break;
    }
    return NULL;
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

    if(cur_func && res){
        
        if(!compare_type(res->type, cur_func->rettype)){
            print_error(8, node->children->next->first_line);
            return NULL;
        }
    }
    return NULL;
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
    //printf("DEFin\n");
    res1 = Def0(node->children);
  
    res2 = DefList0(node->children->next);

    //如果def不行，就直接返回deflist
    if(!res1){
        return res2;
    }


    FieldList fl = res1->fl;
    while(fl->tail){
        fl = fl->tail;
    }
    //def可以，然后把res2接上去过去
    if(res2 && res2->fl){
        //如果下一个定义列表不是空，那就接到def的fl末尾
        fl->tail = res2->fl;
    }
    else
        fl->tail = NULL;
    
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
        fl=fl->tail;
    }
    //传回declist的fl链表
    return res;
}


//当其中一个出现问题，我会把剩下的串起来
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
    fl->lline = node->children->first_line;
    strcpy(fl->name,res->node->val.val_string);
    
    fl->type = res->type;
    fl->tail = NULL;

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

    //都不行就直接返回null
    if(!res1 && !res2)
        return NULL;

    SN *res = malloc(sizeof(SN));
    FieldList fl = malloc(sizeof(struct FieldList_));
    //仅仅declist有问题
    if(!res2){
        fl->lline = node->children->first_line;
        strcpy(fl->name,res1->node->val.val_string);
        fl->type = res1->type;
        fl->tail = NULL;
        res->fl = fl;
        return res;
    }
    else if(!res1){//仅仅dec有问题
        res = res2;
    }
    else{//都没问题
        fl->lline = node->children->first_line;
        strcpy(fl->name,res1->node->val.val_string);
        fl->type = res1->type;
        fl->tail = res2->fl;
        res->fl = fl;
        return res;
    }

    //把构造好的新fl列表传回去
    return res;
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
    res1 = VarDec0(node->children, r);
    return res1;
}

GENF2(Dec, 2){
    SN *res1, *res2;
    res1 = VarDec0(node->children, r);
    res2 = Exp0(node->children->next->next);
    if(!res1 || !res2)
        return NULL;
    
    if(deal_struct){
        char ss[50];
        sprintf(ss,"Error type 15 at Line %d\n", node->children->first_line);
        insert_error_buffer(ss, node->children->first_line);
        //先存起来
        return res1;
    }

    if(!compare_type(res1->type, res2->type)){
        //赋值号两边类型不同
        print_error(5, node->children->first_line);
        return res1;
    }

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

    if(node->children->no != 14
        &&node->children->no != 15
        &&node->children->no != 16)
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

    res1 = malloc(sizeof(SN));
    res1->type = malloc(sizeof(struct Type_));
    res1->type->kind = BASIC;
    res1->type->u.basic = INT;    
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
        //printf("123\n");
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
    res1 = malloc(sizeof(SN));
    res1->type = malloc(sizeof(struct Type_));
    res1->type->kind = BASIC;
    res1->type->u.basic = INT;
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
bool compare_parameter(PTN *lhs, PTN *rhs, int i, char * s){
    if(!lhs && !rhs)
        return true;
    if(!lhs || !rhs)
        return false;

    if(!compare_type(lhs->type, rhs->type))
        return false;

    //递归检查
    return compare_parameter(lhs->next,rhs->next, i+1, s);
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
    if(!res1)
        return NULL;

    if(!arguments_fault){
        if(!compare_parameter(ste->paratype->typelist, res1->ptn, 1, node->children->val.val_string)){
            //参数不匹配
            print_error(9, node->children->first_line);
            return NULL;        
        }
    }
    arguments_fault = false;

    SN* res = (SN *)malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
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
    SN* res = (SN *)malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    res->type = ste->rettype;

    return res; 
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
    }
    
    
    if(res1->type->kind!=ARRAY){
        //不是数组
        print_error(10, node->children->first_line);
        return NULL; 

    }
    res1->type = res1->type->u.array.elem;
    return res1;
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
        //printf("%s\n", node->children->children->val.val_string);
        print_error(13, node->children->first_line);
        return NULL; 
    }
    //printf("123\n");
    FieldList res1 = search_member(res->type->u.structure, node->children->next->next->val.val_string);
    if(!res1){
        //没有这个成员
        print_error(14, node->children->first_line);
        return NULL; 
    }
    res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    memcpy(res->type, res1->type, sizeof(struct Type_));
    return res;

}
GENF(Exp, 16){

    STE *ste = search_entry(node->children->val.val_string);

    bool ispara = false;
    PTN *ptn;
    if(cur_func){
        ptn = cur_func->paratype->typelist;
        while(ptn != NULL){
            if(!strcmp(ptn->name, node->children->val.val_string)){
                ispara = true;
                break;
            }
            ptn = ptn->next;
        }
    }
    if(!ste && !ispara){

        print_error(1,node->children->first_line);
        return NULL;
    }
    //printf("123\n");

    SN *res = malloc(sizeof(SN));
    res->type = malloc(sizeof(struct Type_));
    //在符号表里
    if(ste){
        res->type = ste->type;
        res->valint = 0;
    }
    else{
        res->type = ptn->type;
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

    //在链表头接入类型
    if(res1){
        NEWPTN(res1->type);
        newptn->next = res2->ptn;
        //重复利用res2进行返回
        res2= malloc(sizeof(SN));
        res2->ptn = newptn;
        return res2;
    }else{
        arguments_fault = true;
        return res2;
    }
}
GENF(Args, 2){
    //参数链表尾
    SN *res;
    res = Exp0(node->children);
//printf("123\n");
    if(res){
        NEWPTN(res->type);
        res = malloc(sizeof(SN));
        res->ptn = newptn;
        return res;
    }
    else{
        arguments_fault = true;
        res = malloc(sizeof(SN));
        res->ptn = NULL;
        return res;
    }
}