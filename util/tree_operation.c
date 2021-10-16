#include"tree_operation.h"

struct Node *my_rbtree_search(struct rb_root *root, char *s){
    struct rb_node *node = root->rb_node;
	int res = 0;
    while (node != NULL)
	{
		struct Node *data = container_of(node, struct Node, node);
		
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


bool my_rbtree_insert(struct rb_root *root, struct Node *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
    int res;
	/* Figure out where to put new node */
	while (*new)
	{
		struct Node *this = container_of(*new, struct Node, node);
        
		res = strcmp(data->name, this->name);

		parent = *new;
		if (res < 0)
			new = &((*new)->rb_left);
		else if (res > 0)
			new = &((*new)->rb_right);
		else
			return false;
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return true;
}

bool my_rbtree_remove(struct rb_root *root, char* s){
	struct Node *data = mysearch(root, "walrus");
	if (data != NULL)
	{
		rb_erase(&data->node, root);
		//free
		return true;
	}
	return false;
}