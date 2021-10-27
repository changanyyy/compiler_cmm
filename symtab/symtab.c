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
    insert_entry(newentry);

    return newentry;
}

//初始化红黑树根节点
struct rb_root mytree = RB_ROOT;

//插入表项
bool insert_entry(STE* newentry){
    my_rbtree_insert(&mytree, newentry);
}

//搜索表项
STE* search_entry(char *name){
    return my_rbtree_search(&mytree, name);
}

//删除表项
bool remove_entry(char *name){
    return my_rbtree_remove(&mytree, name);
}





//下面函数用来比较类型


bool compare_struct_field(FieldList lhs, FieldList rhs){
    if(!lhs || !rhs)
        return false;
    bool res;
    res = compare_type(lhs->type, rhs->type);
    if(!res)
        return false;
    
    return compare_struct_field(lhs->tail, rhs->tail);
    
}


 bool compare_type(Type lhs, Type rhs){
    if(lhs==NULL || rhs==NULL)
        return false;
    if(lhs->kind != rhs->kind)
        return false;
    if(lhs->kind == BASIC){
        return lhs->u.basic == rhs->u.basic;
    }
    else if(lhs->kind == ARRAY){
        if(lhs->u.array.size != rhs->u.array.size)
            return false;
        return compare_type(lhs->u.array.elem, rhs->u.array.elem);
    }
    else if(lhs->kind == STRUCT){
        return compare_struct_field(lhs->u.structure, rhs->u.structure);
    }
    else {
        printf("ERROR in compare_type");
        exit(-1);
    }
    
 }