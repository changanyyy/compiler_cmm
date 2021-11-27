#ifndef _SYMTAB_H_
#define _SYMTAB_H_


#include"../util/tree_operation.h"
#include"../include/type.h"
#include"../semantic/semantic.h"
#include"../util/rbtree.h"

//符号表项
struct SymTabEntry{
    
    enum{
        VARIABLE,
        FUNCTION
    } entrytype;
    
    //符号名字
    char name[50];
    bool isdefine;
    bool ispara;
    //一些信息
    Type type;

    int lline;

    //如果是一个函数
    //存储返回值类型和参数类型
    Type rettype;
    struct FunPara *paratype; 
    int var_idx;
    //待定 可能用于存放值
    union{ 
        int valint;
        float valfloat;    
    }val;

    //红黑树节点，这个是linux红黑树的特殊实现
    struct rb_node node;
};

typedef struct SymTabEntry STE;




//创建表项
STE *create_entry(bool isFunction, Type type, struct GTNode *syntaxNode);

//插入表项
bool insert_entry(STE* newentry);

//搜索表项
STE* search_entry(char *name);

//删除表项（可能用不到）
bool remove_entry(char *name);

void scan_fun();

#endif