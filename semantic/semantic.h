#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_


//#include"../include/type.h"
//#include"../include/syntax.h"
//#include"../symtab/symtab.h"


#define CONCAT(arg0, arg1) arg0 ## arg1

#define GENF(name,num) \
    void name##num(struct GTNode *node)


GENF(Program, 0);
GENF(Program, 1);

GENF(ExtDefList, 0);
GENF(ExtDefList, 1);

GENF(ExtDef, 0);
GENF(ExtDef, 1);
GENF(ExtDef, 2);
GENF(ExtDef, 3);


GENF(ExtDecList, 0);
GENF(ExtDecList, 1);
GENF(ExtDecList, 2);

GENF(Specifier, 0);
GENF(Specifier, 1);
GENF(Specifier, 2);


GENF(StructSpecifier, 0);
GENF(StructSpecifier, 1);
GENF(StructSpecifier, 2);


GENF(OptTag, 0);
GENF(OptTag, 1);
GENF(OptTag, 2);

GENF(Tag, 0);
GENF(Tag, 1);


GENF(VarDec, 0);
GENF(VarDec, 1);
GENF(VarDec, 2);


GENF(FunDec, 0);
GENF(FunDec, 1);
GENF(FunDec, 2);


GENF(VarList, 0);
GENF(VarList, 1);
GENF(VarList, 2);

GENF(ParamDec, 0);
GENF(ParamDec, 1);

GENF(CompSt, 0);
GENF(CompSt, 1);

GENF(StmtList, 0);
GENF(StmtList, 1);


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


GENF(DecList, 0);
GENF(DecList, 1);
GENF(DecList, 2);

GENF(Dec, 0);
GENF(Dec, 1);
GENF(Dec, 2);


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






















#endif