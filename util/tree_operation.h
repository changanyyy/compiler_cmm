#ifndef _TREE_OPERATION_H_
#define _TREE_OPERATION_H_



#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"../symtab/symtab.h"
#include"rbtree.h"


struct SymTabEntry *my_rbtree_search(struct rb_root *root, char *s);

bool my_rbtree_insert(struct rb_root *root, struct SymTabEntry *data);

bool my_rbtree_remove(struct rb_root *root, char *s);


#endif