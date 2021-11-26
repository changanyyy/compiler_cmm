#include"ir.h"


#include"../symtab/symtab.h"

//struct InterCode *newInterCodes(int kind, Operand opd1, Operand opd2, Operand opd3, char *label)
struct InterCode *newInterCodes(int kind, ...){

    struct InterCode *res = (struct InterCode *)malloc(sizeof(struct InterCode));

    //初始化种类，把前后指针都设置为0
    res->kind = kind;
    res->pre = res->nxt = NULL;
    
    //变长参数
    va_list argptr;
    va_start(argptr, kind);

    switch(kind){        
    case ASSIGNIR://赋值 lhs := rhs
        res->u.assign.lhs = va_arg(argptr, Operand);
        res->u.assign.rhs = va_arg(argptr, Operand);
        break;
    case ADDIR://加
    case SUBIR://减
    case MULIR://乘
    case DIVIR://除
        //四则运算 res := op1 op op2
        res->u.binop.res = va_arg(argptr, Operand);
        res->u.binop.op1 = va_arg(argptr, Operand);
        res->u.binop.op2 = va_arg(argptr, Operand);
        break;
    case LABELIR://标号语句 LABEL label
//printIC();
        res->u.label = va_arg(argptr, Operand);
        break;
    case GOTOIR://无条件跳转 GOTO gotolabel
        res->u.gotolabel = va_arg(argptr, Operand);
        break;
    case CONDJMPIR://有条件跳转 IF lhs relop rhs GOTO label 
        res->u.cjmp.lhs = va_arg(argptr, Operand);
        res->u.cjmp.relop = va_arg(argptr, int);
        res->u.cjmp.rhs = va_arg(argptr, Operand);
        res->u.cjmp.label = va_arg(argptr, Operand);
        break;
    case READIR://READ read
        res->u.read = va_arg(argptr, Operand);
        break;
    case WRITEIR://WRITE write
        res->u.write = va_arg(argptr, Operand);
        break;
    case CALLIR://res := CALL fun
        res->u.call.res = va_arg(argptr, Operand);
        res->u.call.fun = va_arg(argptr, STE *);
        break;
    case ARGIR://ARG arg
        res->u.arg = va_arg(argptr, Operand);
        break;
    case RETURNIR://RETURN ret
        res->u.ret = va_arg(argptr, Operand);
        break;
    case DECIR://申请空间 DEC x [size]
        res->u.dec.name = va_arg(argptr, STE *);
        res->u.dec.size = va_arg(argptr, int);
        break;
    case FUNIR://函数 FUNCTION fun
        res->u.fun = va_arg(argptr, STE *);
        break;
    case PARAIR://参数 PARA para
        res->u.para = va_arg(argptr, STE *);
        break;
    case ASSIGNADDRIR:
        res->u.aa.res = va_arg(argptr, Operand);
        res->u.aa.var = va_arg(argptr, Operand);
        break;
    case ASSIGNSTARIR:
        res->u.as.res = va_arg(argptr, Operand);
        res->u.as.var = va_arg(argptr, Operand);
        break;
    case STARASSIGNIR:
        res->u.sa.res = va_arg(argptr, Operand);
        res->u.sa.var = va_arg(argptr, Operand);
        break;
    default: break;
    }
    va_end(argptr);

    //连接到双向链表末尾，其中IRhead是额外链表头
    IRhead.pre->nxt = res;
    res->pre = IRhead.pre;
    res->nxt = &IRhead;
    IRhead.pre = res;

    return res;
}


//临时变量
static int numberoftemp = 1;
//label的编号
static int numberoflabel = 1;

//new一个操作数
struct Operand_ *newOperand(int kind, ...){
    Operand opd = (Operand)malloc(sizeof(struct Operand_));
    opd->kind = kind;

    int isint;
    va_list argptr;
    va_start(argptr, kind);
    
    switch(kind){
    //如果操作数是一个变量，那么可以在符号表里面查到
    //(int kind, STE *)
    case VARABLE:
        opd->u.ste = va_arg(argptr, STE *);
        break;
    //如果操作数是一个常量，那么获得它的值
    //第二个参数判断是否是整型数，isint==true代表整型，否则浮点型
    //(int kind, bool isint, int/float)
    case CONSTANT:
        isint = va_arg(argptr, int);//TODO
        opd->isint = isint;
        if(isint) 
            opd->u.int_value = va_arg(argptr, long);
        else 
            opd->u.float_value = va_arg(argptr, double);
        break;
    case ADDRESS:
        opd->u.addr = va_arg(argptr, STE *);
        break;
    case TMPVAR:
        opd->u.tmpnum = numberoftemp++;
        break;
    case LABEL:
        opd->u.labelnum = numberoflabel++;
        break;
    default: break;
    }
    va_end(argptr);
    return opd; 
}

void printOperand(Operand opd){
    switch(opd->kind){
    case VARABLE:
        printf("%s", opd->u.ste->name);
        break;
    case CONSTANT:
        if(opd->isint) 
            printf("#%d", (int)opd->u.int_value);
        else 
            printf("#%f", (float)opd->u.float_value);
        break;
    case ADDRESS:
        //printf("122342342\n");
        printf("&%s", opd->u.addr->name);
        break;
    case TMPVAR:
    //printf("122342342\n");
        printf("t%d", opd->u.tmpnum);
    //printf("\n122342342\n");
        break;
    case LABEL:
        printf("label%d", opd->u.labelnum);
        break;
    }
    return;
}

void printInterCodes(struct InterCode *ir){
    //printf("%d\n", ir->kind);
    switch(ir->kind){        
    case ASSIGNIR://赋值 lhs := rhs
        printOperand(ir->u.assign.lhs);
        printf(" := ");
        printOperand(ir->u.assign.rhs);
        break;
    case ADDIR://加
        printOperand(ir->u.binop.res);
        printf(" := ");
        printOperand(ir->u.binop.op1);
        printf(" + ");
        printOperand(ir->u.binop.op2);
        break;
    case SUBIR://减
        printOperand(ir->u.binop.res);
        printf(" := ");
        printOperand(ir->u.binop.op1);
        printf(" - ");
        printOperand(ir->u.binop.op2);
        break;
    case MULIR://乘
        printOperand(ir->u.binop.res);
        printf(" := ");
        printOperand(ir->u.binop.op1);
        printf(" * ");
        printOperand(ir->u.binop.op2);
        break;
    case DIVIR://除
        printOperand(ir->u.binop.res);
        printf(" := ");
        printOperand(ir->u.binop.op1);
        printf(" / ");
        printOperand(ir->u.binop.op2);
        break;
    case LABELIR://标号语句 LABEL label
        printf("LABEL ");
        printOperand(ir->u.label);
        printf(" :");
        break;
    case GOTOIR://无条件跳转 GOTO gotolabel
        printf("GOTO ");
        printOperand(ir->u.gotolabel);
        break;
    case CONDJMPIR://有条件跳转 IF lhs relop rhs GOTO label
        printf("IF ");
        printOperand(ir->u.cjmp.lhs);
        switch(ir->u.cjmp.relop){
        case 0: printf(" > "); break;
        case 1: printf(" >= "); break;
        case 2: printf(" == "); break;
        case 3: printf(" < "); break;
        case 4: printf(" <= "); break;
        case 5: printf(" != "); break;
        default: exit(0); break;
        }
        printOperand(ir->u.cjmp.rhs);
        printf(" GOTO ");
        printOperand(ir->u.cjmp.label);
        break;
    case READIR://READ read
        printf("READ ");
        printOperand(ir->u.read);
        break;
    case WRITEIR://WRITE write
        printf("WRITE ");
        printOperand(ir->u.write);
        break;
    case CALLIR://res := CALL fun
        printOperand(ir->u.call.res);
        printf(" := CALL ");
        printf("%s", ir->u.call.fun->name);
        break;
    case ARGIR://ARG arg
        printf("ARG ");
        printOperand(ir->u.arg);
        break;
    case RETURNIR://RETURN ret
        printf("RETURN ");
        printOperand(ir->u.ret);
        break;
    case DECIR://申请空间 DEC x [size]
        printf("DEC ");
        printf("%s ", ir->u.dec.name->name);
        printf("%d", ir->u.dec.size);
        break;
    case FUNIR://函数 FUNCTION fun
        printf("FUNCTION ");
        printf("%s :", ir->u.fun->name);
        break;
    case PARAIR://参数 PARA para
        printf("PARAM ");
        printf("%s", ir->u.para->name);
        break;
    case ASSIGNADDRIR:
        printOperand(ir->u.aa.res);
        printf(" := &");
        printOperand(ir->u.aa.var);
        break;
    case ASSIGNSTARIR:
        printOperand(ir->u.as.res);
        printf(" := *");
        printOperand(ir->u.as.var);
        break;
    case STARASSIGNIR:
        printf("*");
        printOperand(ir->u.sa.res);
        printf(" := ");
        printOperand(ir->u.sa.var);
        //res->u.sa.res = va_arg(argptr, Operand);
        //res->u.sa.var = va_arg(argptr, Operand);
        break;
    default: break;
    }
    printf("\n");
}


void printIC(){
    struct InterCode *p = IRhead.nxt;
    while(p != &IRhead){
        printInterCodes(p);
        p = p->nxt;
    }
    return;
}


//下面开始递归解析生成中间代码
void IRProgram0(struct GTNode *node){
    //在生成中间代码之前，进行一些初始化，比如IRhead
    IRhead.pre = IRhead.nxt = &IRhead;

    IRProgram1(node);
}
void IRProgram1(struct GTNode *node){
    IRExtDefList0(node->children);
    return;
}

//外部定义列表
void IRExtDefList0(struct GTNode *node){
    switch(node->no){
    case 1: IRExtDefList1(node); break;
    case 2: IRExtDefList2(node); break;
    default: break;
    }
    return;
}
void IRExtDefList1(struct GTNode *node){
    IRExtDef0(node->children);
    IRExtDefList0(node->children->next);
}
void IRExtDefList2(struct GTNode *node){ }


//其实只有2号有用...
void IRExtDef0(struct GTNode *node){
    switch(node->no){
    case 1: IRExtDef1(node); break;
    case 2: IRExtDef2(node); break;
    case 3: IRExtDef3(node); break;
    case 4: IRExtDef4(node); break;
    default: break;
    }
    return;
}
void IRExtDef1(struct GTNode *node){
    //IRExtDecList0(node->children->next);
}
void IRExtDef2(struct GTNode *node){}
void IRExtDef3(struct GTNode *node){
    IRFunDec0(node->children->next);
    IRCompSt0(node->children->next->next);
}
void IRExtDef4(struct GTNode *node){/*函数声明，忽略*/}

//没有全局变量
/* 
void IRExtDecList0(struct GTNode *node){
    switch(node->no){
    case 1: IRExtDecList1(node); break;
    case 2: IRExtDecList2(node); break;
    default: break;
    }
    return;
}
void IRExtDecList1(struct GTNode *node){
    IRVarDec0(node->children, NULL);
}
void IRExtDecList2(struct GTNode *node){
    IRVarDec0(node->children, NULL);
    IRExtDecList0(node->children->next->next);
}
*/

/*
void IRSpecifier0(struct GTNode *node);
void IRSpecifier1(struct GTNode *node);
void IRSpecifier2(struct GTNode *node);
void IRStructSpecifier0(struct GTNode *node);
void IRStructSpecifier1(struct GTNode *node);
void IRStructSpecifier2(struct GTNode *node);

void IROptTag0(struct GTNode *node);
void IROptTag1(struct GTNode *node);
void IROptTag2(struct GTNode *node);

void IRTag0(struct GTNode *node);
void IRTag1(struct GTNode *node);
*/


STE *CurVarDec = NULL;
//比较关键，变量声明，来源有两个
//函数局部变量声明，函数参数声明
void IRVarDec0(struct GTNode *node, int mode){
    switch(node->no){
    case 1: IRVarDec1(node, mode); break;
    case 2: IRVarDec2(node, mode); break;
    default: break;
    }
    return;
}
void IRVarDec1(struct GTNode *node, int mode){
    STE *ste = search_entry(node->children->val.val_string);
    if(mode == PARAIR)ste->ispara = true;
    CurVarDec = ste;
    //计算数组占用的空间
    if(ste->type->kind == ARRAY && mode == DECIR){
        //局部变量申请空间
        newInterCodes(DECIR, ste, ste->type->u.array.width * ste->type->u.array.size);
    }
    else if(mode == PARAIR){//参数产生参数中间代码
        newInterCodes(PARAIR, ste);
    }
}
void IRVarDec2(struct GTNode *node, int mode){
    IRVarDec0(node->children, mode);//递归进去，找到id
}


void IRFunDec0(struct GTNode *node){
    switch(node->no){
    case 1: IRFunDec1(node); break;
    case 2: IRFunDec2(node); break;
    default: break;
    }
    return;
}
void IRFunDec1(struct GTNode *node){
    STE *ste = search_entry(node->children->val.val_string);
    newInterCodes(FUNIR, ste);
    IRVarList0(node->children->next->next);
    return;
}
void IRFunDec2(struct GTNode *node){
    STE *ste = search_entry(node->children->val.val_string);
    newInterCodes(FUNIR, ste);
}


void IRVarList0(struct GTNode *node){
    switch(node->no){
    case 1: IRVarList1(node); break;
    case 2: IRVarList2(node); break;
    default: break;
    }
    return;
}
void IRVarList1(struct GTNode *node){
    IRParamDec0(node->children);
    IRVarList0(node->children->next->next);
}
void IRVarList2(struct GTNode *node){
    IRParamDec0(node->children);
}


int para = 0;
void IRParamDec0(struct GTNode *node){
    IRParamDec1(node);
}
void IRParamDec1(struct GTNode *node){
    para = 1;
    IRVarDec0(node->children->next, PARAIR);
    para = 0;
}


void IRCompSt0(struct GTNode *node){
    IRCompSt1(node);
}
void IRCompSt1(struct GTNode *node){
    
    IRDefList0(node->children->next);
    IRStmtList0(node->children->next->next);
}

void IRStmtList0(struct GTNode *node){
    switch(node->no){
    case 1: IRStmtList1(node); break;
    case 2: IRStmtList2(node); break;
    default: break;
    }
    return;
}
void IRStmtList1(struct GTNode *node){
    IRStmt0(node->children);
    IRStmtList0(node->children->next);
}
void IRStmtList2(struct GTNode *node){
    //空
}

void IRStmt0(struct GTNode *node){
    //printf("node->no = %d\n", node->no);
    switch(node->no){
    case 1:IRStmt1(node);break;
    case 2:IRStmt2(node);break;
    case 3:IRStmt3(node);break;
    case 4:IRStmt4(node);break;
    case 5:IRStmt5(node);break;
    case 6:IRStmt6(node);break;
    default: break;
    }
    return;
}
void IRStmt1(struct GTNode *node){
    //printf("111\n");
    IRExp0(node->children, NULL);
}
void IRStmt2(struct GTNode *node){
    //printf("11111\n");
    IRCompSt0(node->children);
}
void IRStmt3(struct GTNode *node){
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children->next, t1);
    newInterCodes(RETURNIR, t1);
}
void IRStmt4(struct GTNode *node){
    Operand l1 = newOperand(LABEL);
    Operand l2 = newOperand(LABEL);
    translate_Cond(node->children->next->next, l1, l2);
    newInterCodes(LABELIR, l1);
    IRStmt0(node->children->next->next->next->next);
    newInterCodes(LABELIR, l2);
}
void IRStmt5(struct GTNode *node){
    Operand l1 = newOperand(LABEL);
    Operand l2 = newOperand(LABEL);
    Operand l3 = newOperand(LABEL);
    
    translate_Cond(node->children->next->next, l1, l2);
    newInterCodes(LABELIR, l1);
    IRStmt0(node->children->next->next->next->next);
    newInterCodes(GOTOIR, l3);
    newInterCodes(LABELIR, l2);
    //printf("222222\n");
    IRStmt0(node->children->next->next->next->next->next->next);
    //printf("222222\n");
    //printIC();
    newInterCodes(LABELIR, l3);
}
void IRStmt6(struct GTNode *node){
    Operand l1 = newOperand(LABEL);
    Operand l2 = newOperand(LABEL);
    Operand l3 = newOperand(LABEL);
    newInterCodes(LABELIR, l1);
    translate_Cond(node->children->next->next, l2, l3);
    newInterCodes(LABELIR, l2);
    IRStmt0(node->children->next->next->next->next);
    newInterCodes(GOTOIR, l1);
    newInterCodes(LABELIR, l3);
}


void IRDefList0(struct GTNode *node){
    switch(node->no){
    case 1: IRDefList1(node); break;
    case 2: IRDefList2(node); break;
    default: break;
    }
    return;
}
void IRDefList1(struct GTNode *node){
    IRDef0(node->children);
    IRDefList0(node->children->next);
}
void IRDefList2(struct GTNode *node){/*空*/}


void IRDef0(struct GTNode *node){
    IRDef1(node);
}
void IRDef1(struct GTNode *node){
    IRDecList0(node->children->next);
}

void IRDecList0(struct GTNode *node){
    switch(node->no){
    case 1: IRDecList1(node); break;
    case 2: IRDecList2(node); break;
    default: break;
    }
    return;
}
void IRDecList1(struct GTNode *node){
    IRDec0(node->children);
}
void IRDecList2(struct GTNode *node){
    IRDec0(node->children);
    IRDecList0(node->children->next->next);
}

void IRDec0(struct GTNode *node){
    switch(node->no){
    case 1: IRDec1(node); break;
    case 2: IRDec2(node); break;
    }
    return;
}
void IRDec1(struct GTNode *node){
    IRVarDec0(node->children, DECIR);
    //printIC();
}
void IRDec2(struct GTNode *node){
    IRVarDec0(node->children, DECIR);
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children->next->next, t1);
    Operand res = newOperand(VARABLE, CurVarDec);
    CurVarDec = NULL;
    newInterCodes(ASSIGNIR, res, t1);
}


Type IRExp0(struct GTNode *node,Operand place){
    
    Type t = NULL;
    switch(node->no){
    case 1:  IRExp1(node, place);break;
    case 2:  IRExp2(node, place);break;
    case 3:  IRExp3(node, place);break;
    case 4:  IRExp4(node, place);break;
    case 5:  IRExp5(node, place);break;
    case 6:  IRExp6(node, place);break;
    case 7:  IRExp7(node, place);break;
    case 8:  IRExp8(node, place);break;
    case 9:  IRExp9(node, place);break;
    case 10: IRExp10(node, place);break;
    case 11: IRExp11(node, place);break;
    case 12: IRExp12(node, place);break;
    case 13: IRExp13(node, place);break;
    case 14: t = IRExp14(node, place);break;
    case 15: IRExp15(node);break;
    case 16: t = IRExp16(node, place);break;
    case 17: IRExp17(node, place);break;
    case 18: IRExp18(node, place);break;
    default: break;
    }
    return t;
}

int deal_array_IR = 0;

void IRExp1(struct GTNode *node, Operand place){
    STE *ste;
    Operand t1, t2, t3, var;
    switch (node->children->no)
    {
    case 14://Exp1->array
    //TODO
        t1 = newOperand(TMPVAR);
        t2 = newOperand(TMPVAR);
        deal_array_IR = 1;
        IRExp0(node->children, t1);
        deal_array_IR = 0;
        IRExp0(node->children->next->next, t2);
        newInterCodes(STARASSIGNIR, t1, t2);
        if(place)
            newInterCodes(ASSIGNIR, place, t2);
        break;
    case 15://Exp1->structure
    //TODO
        break;
    case 16://Exp1->id
        ste = search_entry(node->children->children->val.val_string);//搜索表项
        t1 = newOperand(TMPVAR);
        IRExp0(node->children->next->next, t1);
        var = newOperand(VARABLE, ste);
        newInterCodes(ASSIGNIR, var, t1);

        if(place)
            newInterCodes(ASSIGNIR, place, var);
        break;
    default:
        break;
    }
    return;
}
void IRExp2(struct GTNode *node, Operand place){
    if(!place)
        return;
    Operand l1 = newOperand(LABEL);
    Operand l2 = newOperand(LABEL);
    Operand zero = newOperand(CONSTANT, 1, 0);
    Operand one = newOperand(CONSTANT, 1, 1);
    
    newInterCodes(ASSIGNIR, place, zero);
    translate_Cond(node, l1, l2);
    newInterCodes(LABELIR, l1);
    newInterCodes(ASSIGNIR, place, one);
    newInterCodes(LABELIR, l2);
}
void IRExp3(struct GTNode *node, Operand place){
    IRExp2(node, place);
}
void IRExp4(struct GTNode *node, Operand place){
    IRExp2(node, place);
}

//下面是表达式的加减乘除四则运算
//加法
void IRExp5(struct GTNode *node, Operand place){
    if(!place)
        return;
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    newInterCodes(ADDIR, place, t1, t2);
    return;
}
//减法
void IRExp6(struct GTNode *node, Operand place){
    if(!place)
        return;
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    newInterCodes(SUBIR, place, t1, t2);
    return;
}
//乘法
void IRExp7(struct GTNode *node, Operand place){
    if(!place)
        return;
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    newInterCodes(MULIR, place, t1, t2);
    return;
}
//除法
void IRExp8(struct GTNode *node, Operand place){
    if(!place)
        return;    
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    newInterCodes(DIVIR, place, t1, t2);
    return;
}

void IRExp9(struct GTNode *node, Operand place){
    IRExp0(node->children->next, place);
}
void IRExp10(struct GTNode *node, Operand place){
    if(!place)
        return;
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children->next, t1);
    Operand zero = newOperand(CONSTANT, 1, 0);
    newInterCodes(SUBIR, place, zero, t1);
}
void IRExp11(struct GTNode *node, Operand place){
    IRExp2(node, place);
}
void IRExp12(struct GTNode *node, Operand place){//ID LP Args RP    
    STE *ste = search_entry(node->children->val.val_string);
    Operand arglist;
    IRArgs0(node->children->next->next, &arglist);

    if(!strcmp(ste->name, "write")){
        
        newInterCodes(WRITEIR, arglist);
        Operand zero = newOperand(CONSTANT, 1, 0);
        return;
    }
    for(int i = 0; i < ste->paratype->paranum; i++){
        newInterCodes(ARGIR, arglist);
        arglist = arglist->nxt;
    }
    if(place)
        newInterCodes(CALLIR, place, ste);
    else{
        Operand ret = newOperand(TMPVAR);
        newInterCodes(CALLIR, ret, ste);
    }
    return;
}
void IRExp13(struct GTNode *node, Operand place){//ID LP RP
    if(!place)
        return;
    STE *ste = search_entry(node->children->val.val_string);
    if(!ste){
        printf("Error ir 324\n");
    }
    if(!strcmp(ste->name, "read")){
        newInterCodes(READIR, place);
        return;
    }
    newInterCodes(CALLIR, place, ste);
}



Type IRExp14(struct GTNode *node, Operand place){//Exp LB Exp RB
    //printf("kkk\n");
    if(!place)
        return NULL; 
    Type type;
    Operand t1 = newOperand(TMPVAR);
    type = IRExp0(node->children, t1);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children->next->next, t2);
    Operand wid = newOperand(CONSTANT, 1, type->u.array.width);
    //计算宽度
    newInterCodes(MULIR, t2, t2, wid);
    //加到前面的计算值上
    if(deal_array_IR){
        newInterCodes(ADDIR, place, t1, t2);
    }
    else{
        //printf("%d\n", type->kind);
        newInterCodes(ADDIR, place, t1, t2);
        if(type->u.array.elem->kind != ARRAY){
            newInterCodes(ASSIGNSTARIR, place, place);
        }
    }

    //返回下一层元素
    return type->u.array.elem;
}
void IRExp15(struct GTNode *node){ }//Exp DOT ID

Type IRExp16(struct GTNode *node, Operand place){//ID
    if(!place)
        return NULL;
    STE *ste = search_entry(node->children->val.val_string);
    Operand opd;
    if(ste->type->kind == ARRAY && !ste->ispara){
        opd = newOperand(ADDRESS, ste);
    }else{
        opd = newOperand(VARABLE, ste);
    }
    newInterCodes(ASSIGNIR, place, opd);
    return ste->type;
}
void IRExp17(struct GTNode *node, Operand place){
    if(!place)
        return;
    long val = node->children->val.val_int;
    Operand i = newOperand(CONSTANT, 1, val);
    newInterCodes(ASSIGNIR, place, i);
    return;
}
void IRExp18(struct GTNode *node, Operand place){
    if(!place)
        return;
    double val = node->children->val.val_float;
    Operand i = newOperand(CONSTANT, 0, val);
    newInterCodes(ASSIGNIR, place, i);
    return;
}

void IRArgs0(struct GTNode *node, Operand *arglist){
    switch (node->no)
    {
    case 1: IRArgs1(node, arglist); break;
    case 2: IRArgs2(node, arglist); break;
    default: break;
    }
}
void IRArgs1(struct GTNode *node, Operand *arglist){
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    t1->nxt = *arglist;// arglist = t1 + arglist
    *arglist = t1;
    IRArgs0(node->children->next->next, arglist);
}
void IRArgs2(struct GTNode *node, Operand *arglist){
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    t1->nxt = *arglist;// arglist = t1 + arglist
    *arglist = t1;
}



void translate_Cond(struct GTNode *node, Operand label_true, Operand label_false){
    Operand l1, l2, l3;
    int relop;
    Operand t1, t2, zero;
    switch(node->no){
    case 2://Exp AND Exp
        l1 = newOperand(LABEL);
        translate_Cond(node->children, l1, label_false);
        newInterCodes(LABELIR, l1);
        translate_Cond(node->children->next->next, label_true, label_false);
        break;
    case 3://Exp OR Exp
        l1 = newOperand(LABEL);
        translate_Cond(node->children, label_true, l1);
        newInterCodes(LABELIR, l1);
        translate_Cond(node->children->next->next, label_true, label_false);
        break;
    case 4://Exp RELOP Exp
        if(!strcmp("<=", node->children->next->val.val_string))
            relop = LE;
        else if(!strcmp("<", node->children->next->val.val_string))
            relop = L;
        else if(!strcmp("==", node->children->next->val.val_string))
            relop = EQ;
        else if(!strcmp("!=", node->children->next->val.val_string))
            relop = NE;
        else if(!strcmp(">=", node->children->next->val.val_string))
            relop = GE;
        else if(!strcmp(">", node->children->next->val.val_string))
            relop = G;
        t1 = newOperand(TMPVAR);
        t2 = newOperand(TMPVAR);
        IRExp0(node->children, t1);
        IRExp0(node->children->next->next, t2);
        newInterCodes(CONDJMPIR, t1, relop, t2, label_true);
        newInterCodes(GOTOIR, label_false);
        break;
    case 11://NOT Exp 
        translate_Cond(node->children->next, label_false, label_true);
        break;
    default://other cases
        t1 = newOperand(TMPVAR);
        IRExp0(node, t1);
        zero = newOperand(CONSTANT, 1, 0);
        newInterCodes(CONDJMPIR, t1, NE, zero, label_true);
        newInterCodes(GOTOIR, label_false);
        break;
    }
    return;
}