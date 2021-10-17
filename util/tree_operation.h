#ifndef _TREE_OPERATION_H_
#define _TREE_OPERATION_H_


#include"../include/syntax.h"
#include"rbtree.h"

#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"../symtab/symtab.h"



STE *my_rbtree_search(struct rb_root *root, char *s);

bool my_rbtree_insert(struct rb_root *root, STE *data);

bool my_rbtree_remove(struct rb_root *root, char* s);


#endif