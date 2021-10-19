#include"symtab.h"
#include<stdlib.h>
#include"../include/syntax.h"
#include"../syntax.tab.h"

//创建一个表项，返回表项指针
STE *create_entry(bool isFunction, Type type, struct GTNode *syntaxNode){
    //new一个新表项
    STE *newentry = (STE *)malloc(sizeof(STE));
    memset(newentry, '\0', sizeof(STE));
    
    //表项名字
    strcpy(newentry->name, syntaxNode->val.val_string);
    //表项的类型：函数 or 变量
    newentry->entrytype = isFunction?FUNCTION:VARIABLE;


    //基础类型还会有值
    if(!isFunction){
        //如果是变量的话还会产生类型
        newentry->type = type;
        if(syntaxNode->type == FLOAT){
            newentry->val.valfloat =  syntaxNode->val.val_float;
        }
        else if(syntaxNode->type == INT){
            newentry->val.valint = syntaxNode->val.val_int;
        }
    }

    return newentry;
}


//插入表项
bool insert_entry(STE* newentry){
    my_rbtree_insert(&mytree, newentry);
}

//
STE* search_entry(const char *name){
    return my_rbtree_search(&mytree, name);
}

//删除表项
bool remove_entry(const char *name){
    return my_rbtree_remove(&mytree, name);
}