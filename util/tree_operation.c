#include"tree_operation.h"
#include<stdlib.h>

STE *my_rbtree_search(struct rb_root *root, char *s){
    struct rb_node *node = root->rb_node;
	int res = 0;
    while (node != NULL)
	{
		//找到包含这个node节点（根节点）的STE（symbol table entry）
		STE *data = container_of(node, STE, node);
		
		//对名字进行比较，平衡二叉树经典操作
		res = strcmp(s, data->name);
		if (res < 0)
			node = node->rb_left;
		else if (res > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}


bool my_rbtree_insert(struct rb_root *root, STE *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
    int res;

	//找到新节点应该插入到哪里
	//二级指针代表父节点的子女指针
	//因为*new可能是NULL，所以用二级指针
	while (*new)
	{
		STE *this = container_of(*new, STE, node);
        
		res = strcmp(data->name, this->name);

		//仍然二分查找
		parent = *new;
		if (res < 0){
			new = &((*new)->rb_left);
		}
		else if (res > 0){
			new = &((*new)->rb_right);
		}
		else{
			//如果已经有同名表项，那就失败
			return false;
		}
	}

	//在new处添加节点data->node，父亲指针是parent
	rb_link_node(&data->node, parent, new);
	//恢复平衡
	rb_insert_color(&data->node, root);

	return true;
}

bool my_rbtree_remove(struct rb_root *root, char* s){
	STE *data = my_rbtree_search(root, s);
	if (data != NULL)
	{
		rb_erase(&data->node, root);
		//释放刚刚开辟的空间
		free(data);
		return true;
	}
	return false;
}