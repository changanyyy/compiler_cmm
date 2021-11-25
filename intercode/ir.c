#include"ir.h"


#include"../symtab/symtab.h"

//struct InterCode *newInterCodes(int kind, Operand opd1, Operand opd2, Operand opd3, char *label)
struct InterCode *newInterCodes(int kind, ...){
    struct InterCode *res = (struct InterCode *)malloc(sizeof(struct InterCode));

    res->kind = kind;
    res->pre = res->nxt = NULL;
    
    va_list argptr;
    va_start(argptr, kind);
    switch(kind){        
    case ASSIGNIR: 
        res->u.assign.lhs = va_arg(argptr, Operand);
        res->u.assign.rhs = va_arg(argptr, Operand);
        break;
    case ADDIR:
    case SUBIR: 
    case MULIR: 
    case DIVIR:  
        res->u.binop.res = va_arg(argptr, Operand);
        res->u.binop.op1 = va_arg(argptr, Operand);
        res->u.binop.op2 = va_arg(argptr, Operand);
        break;
    case LABELIR:
        res->u.label = va_arg(argptr, Operand);
        break;
    case GOTOIR:
        res->u.gotolabel = va_arg(argptr, Operand);
        break;
    case CONDJMPIR:
        res->u.cjmp.lhs = va_arg(argptr, Operand);
        res->u.cjmp.relop = va_arg(argptr, int);
        res->u.cjmp.rhs = va_arg(argptr, Operand);
        res->u.cjmp.label = va_arg(argptr, Operand);
        break;
    case READIR:
        res->u.read = va_arg(argptr, Operand);
        break;
    case WRITEIR:
        res->u.write = va_arg(argptr, Operand);
        break;
    case CALLIR:
        res->u.call.res = va_arg(argptr, Operand);
        res->u.call.fun = va_arg(argptr, STE *);
        break;
    case ARGIR:
        res->u.arg = va_arg(argptr, Operand);
        break;
    case RETURNIR:
        res->u.ret = va_arg(argptr, Operand);
        break;
    case DECIR:
        res->u.dec.name = va_arg(argptr, STE *);
        res->u.dec.size = va_arg(argptr, Operand);
        break;
    case FUNIR:
        res->u.fun = va_arg(argptr, STE *);
        break;
    case PARAIR:
        res->u.para = va_arg(argptr, STE *);
        break;
    default: break;
    }
    va_end(argptr);

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

    bool isint;
    va_list argptr;
    va_start(argptr, kind);
    
    switch(kind){
    //如果操作数是一个变量，那么可以在符号表里面查到
    //(int kind, STE *)
    case VARIABLE:
        opd->u.ste = va_arg(argptr, STE *);
        break;
    //如果操作数是一个常量，那么获得它的值
    //第二个参数判断是否是整型数，isint==true代表整型，否则浮点型
    //(int kind, bool isint, int/float)
    case CONSTANT:
        isint = va_arg(argptr, bool);
        if(isint) 
            opd->u.int_value = va_arg(argptr, long);
        else 
            opd->u.float_value = va_arg(argptr, double);
        break;
    case ADDRESS:
        opd->u.ste = va_arg(argptr, STE *);
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



//下面开始递归解析生成中间代码
void IRProgram0(struct GTNode *node){
    switch(node->no){
    case 1: IRProgram1(node); break;
    default: break;
    }
    return;
}
void IRProgram1(struct GTNode *node){
    IRExtDecList0(node->children);
    return;
}

void IRExtDefList0(struct GTNode *node){
    switch(node->no){
    case 1: IRExtDecList0(node);
    case 2: IRExtDecList0(node);
    default: break;
    }
    return;
}
void IRExtDefList1(struct GTNode *node){
    IRExtDef(node->children);
    IRExtDecList0(node->children->next);
}
void IRExtDefList2(struct GTNode *node){
    //空
}

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
    IRExtDecList0(node->children->next);
}
void IRExtDef2(struct GTNode *node){

}
void IRExtDef3(struct GTNode *node){
    IRFunDec0(node->children->next);
    CompSt0(node->children->next->next);
}
void IRExtDef4(struct GTNode *node){
    //函数声明，不在
    //IRFunDec0(node->children->next);
}


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
    //计算数组占用的空间
    Operand size = newOperand(CONSTANT, true, ste->type->u.array.width * ste->type->u.array.size);
    if(mode == DECIR){
        newInterCodes(DECIR, ste, size);
    }
    else if(mode == PARAIR){
        newInterCodes(PARAIR, ste);
    }
}
void IRVarDec2(struct GTNode *node, int mode){
    IRVarDec0(node->children, mode);
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
    IRParamDec0(node);
}

void IRParamDec0(struct GTNode *node){
    switch(node->no){
    case 1: IRParamDec1(node); break;
    default: break;
    }
    return;
}
void IRParamDec1(struct GTNode *node){
    VarDec0(node->children->next, PARAIR);
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
    switch(node->no){
    case 1:IRStmt1(node);break;
    case 2:IRStmt1(node);break;
    case 3:IRStmt1(node);break;
    case 4:IRStmt1(node);break;
    case 5:IRStmt1(node);break;
    case 6:IRStmt1(node);break;
    }
    return;
}
void IRStmt1(struct GTNode *node){
    IRExp0(node->children, NULL);
}
void IRStmt2(struct GTNode *node){
    CompSt0(node->children);
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
    IRStmt0(node->children->next->next->next->next->next->next);
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
void IRDefList2(struct GTNode *node){
    //空
}


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
    IRVarDec0(node->children, Dec0);
}
void IRDecList2(struct GTNode *node){
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children->next->next, t1);
    
}

void IRDec0(struct GTNode *node);
void IRDec1(struct GTNode *node);
void IRDec2(struct GTNode *node);


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
void IRExp1(struct GTNode *node, Operand place){
    STE *ste;
    switch (node->children->no)
    {
    case 14://Exp1->array
    //TODO

        break;
    case 15://Exp1->structure
    //TODO
        break;
    case 16://Exp1->id
        ste = search_entry(node->children->children->val.val_string);//搜索表项
        Operand t = newOperand(TMPVAR);
        IRExp0(node->children->next->next, t);
        Operand var = newOperand(VARIABLE, ste);
        newInterCodes(ASSIGNIR, var, t);
        if(place)
            newInterCodes(ASSIGNIR, place, var);
        
        break;
    default:
        break;
    }
    return;
}
void IRExp2(struct GTNode *node, Operand place){
    Operand l1 = newOperand(LABEL);
    Operand l2 = newOperand(LABEL);
    Operand zero = newOperand(CONSTANT, true, 0);
    Operand one = newOperand(CONSTANT, true, 1);
    newInterCodes(ASSIGNIR, place, zero);
    translate_Cond(node, l1, l2);
    newInterCodes(LABELIR, l1);
    newInterCodes(ASSIGNIR, place, 1);
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
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    if(place)
        newInterCodes(ADDIR, place, t1, t2);
    return;
}
//减法
void IRExp6(struct GTNode *node, Operand place){
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    if(place)
        newInterCodes(SUBIR, place, t1, t2);
    return;
}
//乘法
void IRExp7(struct GTNode *node, Operand place){
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    if(place)
        newInterCodes(MULIR, place, t1, t2);
    return;
}
//除法
void IRExp8(struct GTNode *node, Operand place){
    Operand t1 = newOperand(TMPVAR);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children, t1);
    IRExp0(node->children->next->next, t2);
    if(place)
        newInterCodes(DIVIR, place, t1, t2);
    return;
}

void IRExp9(struct GTNode *node, Operand place){
    IRExp0(node, place);
}
void IRExp10(struct GTNode *node, Operand place){
    Operand t1 = newOperand(TMPVAR);
    IRExp0(node->children->next, t1);
    Operand zero = newOperand(CONSTANT, true, 0);
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
        Operand zero = newOperand(CONSTANT, true, 0);
        newInterCodes(ASSIGNIR, place, zero);
        return;
    }
    for(int i = 0; i < ste->paratype->paranum; i++){
        newInterCodes(ARGIR, arglist);
        arglist = arglist->nxt;
    }
    newInterCodes(CALLIR, place, ste);
    return;
}
void IRExp13(struct GTNode *node, Operand place){//ID LP RP
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

//STE *curarr = NULL;

Type IRExp14(struct GTNode *node, Operand place){//Exp LB Exp RB
    Type type;
    Operand t1 = newOperand(TMPVAR);
    type = IRExp0(node->children, t1);
    Operand t2 = newOperand(TMPVAR);
    IRExp0(node->children->next->next, t2);
    Operand wid = newOperand(CONSTANT, true, type->u.array.width);
    //计算宽度
    newInterCodes(MULIR, t2, t2, wid);
    //加到前面的计算值上
    newInterCodes(ADDIR, t1, t1, t2);
    //返回下一层元素
    return type->u.array.elem;
}
void IRExp15(struct GTNode *node);//Exp DOT ID

Type IRExp16(struct GTNode *node, Operand place){//ID
    STE *ste = search_entry(node->children->val.val_string);
    Operand opd = newOperand(ADDRESS, ste);
    newInterCodes(ASSIGNIR, place, opd);
    return ste->type;
}
void IRExp17(struct GTNode *node, Operand place){
    long val = node->children->val.val_int;
    Operand i = newOperand(CONSTANT, true, val);
    newInterCodes(ASSIGNIR, place, i);
    return;
}
void IRExp18(struct GTNode *node, Operand place){
    double val = node->children->val.val_float;
    Operand i = newOperand(CONSTANT, false, val);
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
    switch(node->no){
    case 2://Exp AND Exp
        Operand l1 = newOperand(LABEL);
        translate_Cond(node->children, l1, label_false);
        newInterCodes(LABELIR, l1);
        translate_Cond(node->children->next->next, label_true, label_false);
        break;
    case 3://Exp OR Exp
        Operand l1 = newOperand(LABEL);
        translate_Cond(node->children, label_true, l1);
        newInterCodes(LABELIR, l1);
        translate_Cond(node->children->next->next, label_true, label_false);
        break;
    case 4://Exp RELOP Exp
        int relop;
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
        Operand t1 = newOperand(TMPVAR);
        Operand t2 = newOperand(TMPVAR);
        IRExp0(node->children, t1);
        IRExp0(node->children->next->next, t2);
        newInterCodes(CONDJMPIR, t1, relop, t2, label_true);
        newInterCodes(GOTOIR, label_false);
        break;
    case 11://NOT Exp 
        translate_Cond(node->children->next, label_false, label_true);
        break;
    default://other cases
        Operand t1 = newOperand(TMPVAR);
        IRExp0(node, t1);
        Operand zero = newOperand(CONSTANT, true, 0);
        newInterCodes(CONDJMPIR, t1, NE, zero, label_true);
        newInterCodes(GOTOIR, label_false);
        break;
    }
    return;
}