#include"symtab.h"
#include<stdlib.h>
#include"../include/syntax.h"
#include"../syntax.tab.h"



//创建一个表项，返回表项指针
STE *create_entry(bool isFunction, Type type, struct GTNode *syntaxNode){
    //printf("Create Entry: %s\n", syntaxNode->val.val_string);


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

//int cnt = 0;
bool compare_struct_field(FieldList lhs, FieldList rhs){
    //printf("cnt= %d\n", cnt++);
    if(!lhs && !rhs)
        return true;
    if(!lhs || !rhs)
        return false;
    bool res;
    res = compare_type(lhs->type, rhs->type);
    if(!res)
        return false;
    else
        return compare_struct_field(lhs->tail, rhs->tail);
    
}


 bool compare_type(Type lhs, Type rhs){
    if(!lhs && !rhs)
        return true;
    if(lhs==NULL || rhs==NULL)
        return false;
    if(lhs->kind != rhs->kind)
        return false;
    if(lhs->kind == BASIC){
        return lhs->u.basic == rhs->u.basic;
    }
    else if(lhs->kind == ARRAY){
        //if(lhs->u.array.size != rhs->u.array.size)
        //    return false;
        return compare_type(lhs->u.array.elem, rhs->u.array.elem);
    }
    else if(lhs->kind == STRUCTURE){
        return compare_struct_field(lhs->u.structure, rhs->u.structure);
    }
    else {
        printf("ERROR in compare_type");
        exit(-1);
    }
    
 }


void print_type1(Type this, int tab){
    if(!this)
        return;
    for(int i = 0; i < tab; i++){
        printf("  ");
    }
    if(this->kind == BASIC){
        printf("BASIC TYPE: ");
        if(this->u.basic == INT){
            printf("INT\n");
        }
        else{
            printf("FLOAT\n");
        }
    }
    else if(this->kind == STRUCTURE){
        printf("STRUCTURE:\n");
        FieldList fl = this->u.structure;
        while(fl){
            printf("Member: %s ",fl->name);
            print_type1(fl->type, tab);
            fl=fl->tail;
        }
    }
    else if(this->kind == ARRAY){
        printf("ARRAY: Dim = %d, element:\n", this->u.array.size);

        print_type1(this->u.array.elem, tab+1);
    }
}

void print_type(Type this){
    printf("\nType:\n");
    print_type1(this, 0);
    printf("\n");     
}