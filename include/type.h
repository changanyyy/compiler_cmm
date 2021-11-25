#ifndef _TYPE_H_
#define _TYPE_H_

#include<stdbool.h>

/*
*定义了类型相关的结构和比较类型的方法
*/

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_{
    enum { BASIC, ARRAY, STRUCTURE, SPECIAL} kind;
    
    union{
        // 基本类型
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct { Type elem; int size;int width; } array;
        // 结构体类型信息是一个链表
        FieldList structure;
    } u;
};

struct FieldList_
{
    char name[50]; // 域的名字
    Type type; // 域的类型 
    FieldList tail; // 下一个域
    int lline;
};


//比较类型是否相同，递归比较
bool compare_type(Type lhs, Type rhs);

void print_type(Type this);



#endif