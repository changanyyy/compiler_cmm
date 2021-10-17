#ifndef _SYMTAB_H_
#define _SYMTAB_H_
#include"../util/tree_operation.h"
#include"../include/type.h"

//符号表项
typedef struct SymTabEntry{
    
    enum{
        VARIABLE,
        FUNCTION
    } entrytype;
    
    //符号名字
    char name[50];
    //是否已经被定义
    bool isdefined;
    //一些信息
    Type *type;

    union{
        int valint;
        int valfloat;    
    }val;

    struct rb_node node;
} STE;



struct rb_root mytree = RB_ROOT;



STE *create_entry(bool isFunction, Type *type, struct GTNode *syntaxNode);

bool insert_entry(STE* newentry);

STE* search_entry(const char *name);

bool remove_entry(const char *name);


#endif