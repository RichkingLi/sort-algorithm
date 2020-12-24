#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED				1
#define BLACK 			2

typedef int KEY_TYPE;

// 红黑树的节点
typedef struct _rbtree_node {
	unsigned char color;				// 颜色(RED 或 BLACK)
	struct _rbtree_node *right;			// 右孩子
	struct _rbtree_node *left;			// 左孩子
	struct _rbtree_node *parent;		// 父结点
	KEY_TYPE key;						// 关键字(键值)
	void *value;
} rbtree_node;

// 红黑树的根
typedef struct _rbtree {
	rbtree_node *root;
	rbtree_node *nil;
} rbtree;

rbtree_node *rbtree_mini(rbtree *T, rbtree_node *x) {
	while (x->left != T->nil) {
		x = x->left;
	}
	return x;
}

rbtree_node *rbtree_maxi(rbtree *T, rbtree_node *x) {
	while (x->right != T->nil) {
		x = x->right;
	}
	return x;
}

rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x) {
	rbtree_node *y = x->parent;

	if (x->right != T->nil) {
		return rbtree_mini(T, x->right);
	}

	while ((y != T->nil) && (x == y->right)) {
		x = y;
		y = y->parent;
	}
	return y;
}


void rbtree_left_rotate(rbtree *T, rbtree_node *x) {
	
	rbtree_node *y = x->right;  // 设置x的右孩子为y

	
	x->right = y->left; 		// 将 “y的左孩子” 设为 “x的右孩子”；
	if (y->left != T->nil) { 	// 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
		y->left->parent = x;
	}

	y->parent = x->parent;		// 将 “x的父亲” 设为 “y的父亲”
	if (x->parent == T->nil) { 	// 如果 “x的父亲” 是空节点，则将y设为根节点
		T->root = y;
	} else if (x == x->parent->left) {// 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
		x->parent->left = y;
	} else {	// 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
		x->parent->right = y;
	}

	y->left = x; 	// 将 “x” 设为 “y的左孩子”
	x->parent = y; 	// 将 “x的父节点” 设为 “y”
}


void rbtree_right_rotate(rbtree *T, rbtree_node *y) {

	rbtree_node *x = y->left;

	y->left = x->right;
	if (x->right != T->nil) {
		x->right->parent = y;
	}

	x->parent = y->parent;
	if (y->parent == T->nil) {
		T->root = x;
	} else if (y == y->parent->right) {
		y->parent->right = x;
	} else {
		y->parent->left = x;
	}

	x->right = y;
	y->parent = x;
}

void rbtree_insert_fixup(rbtree *T, rbtree_node *z) {

	while (z->parent->color == RED) { // 若“父节点的颜色是红色”
		if (z->parent == z->parent->parent->left) {	//若“父节点”是“祖父节点的左孩子”
			rbtree_node *y = z->parent->parent->right;
			if (y->color == RED) {	// Case 1条件：叔叔节点是红色
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent; //z --> RED
			} else {	

				if (z == z->parent->right) {	// Case 2条件：叔叔是黑色，且当前节点是右孩子
					z = z->parent;
					rbtree_left_rotate(T, z);
				}

				// Case 3条件：叔叔是黑色，且当前节点是左孩子。
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_right_rotate(T, z->parent->parent);
			}
		}else {	//若“z的父节点”是“z的祖父节点的右孩子”
			rbtree_node *y = z->parent->parent->left;
			if (y->color == RED) { // Case 1条件：叔叔节点是红色
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;

				z = z->parent->parent; //z --> RED
			} else {
				if (z == z->parent->left) {// Case 2条件：叔叔是黑色，且当前节点是左孩子
					z = z->parent;
					rbtree_right_rotate(T, z);
				}

				// Case 3条件：叔叔是黑色，且当前节点是右孩子。
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rbtree_left_rotate(T, z->parent->parent);
			}
		}
		
	}

	T->root->color = BLACK;
}


void rbtree_insert(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;
	rbtree_node *x = T->root;
	// 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
	while (x != T->nil) {
		y = x;
		if (z->key < x->key) {
			x = x->left;
		} else if (z->key > x->key) {
			x = x->right;
		} else { //Exist
			return ;
		}
	}

	z->parent = y;
	if (y == T->nil) {
		T->root = z;
	} else if (z->key < y->key) {
		y->left = z;
	} else {
		y->right = z;
	}

	z->left = T->nil;
	z->right = T->nil;
	z->color = RED;		// 2. 设置节点的颜色为红色

	rbtree_insert_fixup(T, z);	// 3. 将它重新修正为一颗红黑树
}

void rbtree_delete_fixup(rbtree *T, rbtree_node *x) {

	while ((x != T->root) && (x->color == BLACK)) {
		if (x == x->parent->left) {

			rbtree_node *w= x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				rbtree_left_rotate(T, x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->right->color == BLACK) {
					w->left->color = BLACK;
					w->color = RED;
					rbtree_right_rotate(T, w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				rbtree_left_rotate(T, x->parent);

				x = T->root;
			}

		} else {

			rbtree_node *w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;
				rbtree_right_rotate(T, x->parent);
				w = x->parent->left;
			}

			if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
				w->color = RED;
				x = x->parent;
			} else {

				if (w->left->color == BLACK) {
					w->right->color = BLACK;
					w->color = RED;
					rbtree_left_rotate(T, w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				rbtree_right_rotate(T, x->parent);

				x = T->root;
			}

		}
	}

	x->color = BLACK;
}

rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z) {

	rbtree_node *y = T->nil;
	rbtree_node *x = T->nil;
	
	// 被删除节点有一个或没有儿子，不用寻找后继
	if ((z->left == T->nil) || (z->right == T->nil)) {
		y = z;
	} else { //被删除节点有2个儿子，需要寻找后继
		y = rbtree_successor(T, z);	
	}

	//如果后继有儿子
	if (y->left != T->nil) {
		x = y->left;
	} else if (y->right != T->nil) {
		x = y->right;
	}
	x->parent = y->parent;//将“后继的儿子的父亲”设置为“后继的父亲”
	
	if (y->parent == T->nil) {//设置“后继的兄弟”为x
		T->root = x;
	} else if (y == y->parent->left) {
		y->parent->left = x;
	} else {
		y->parent->right = x;
	}
	//y的东西过继给z
	if (y != z) {
		z->key = y->key;
		z->value = y->value;
	}
	
	if (y->color == BLACK) {
		rbtree_delete_fixup(T, x);
	}

	return y;
}

rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key) {

	rbtree_node *node = T->root;
	while (node != T->nil) {
		if (key < node->key) {
			node = node->left;
		} else if (key > node->key) {
			node = node->right;
		} else {
			return node;
		}	
	}
	return T->nil;
}


void rbtree_traversal(rbtree *T, rbtree_node *node) {
	if (node != T->nil) {
		rbtree_traversal(T, node->left);
		printf("key:%d, color:%d\n", node->key, node->color);
		rbtree_traversal(T, node->right);
	}
}

int main() {

	int keyArray[20] = {24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15};

	rbtree *T = (rbtree *)malloc(sizeof(rbtree));
	if (T == NULL) {
		printf("malloc failed\n");
		return -1;
	}
	
	T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
	T->nil->color = BLACK;
	T->root = T->nil;

	rbtree_node *node = T->nil;
	int i = 0;
	for (i = 0;i < 20;i ++) {
		node = (rbtree_node*)malloc(sizeof(rbtree_node));
		node->key = keyArray[i];
		node->value = NULL;

		rbtree_insert(T, node);
		
	}

	rbtree_traversal(T, T->root);
	printf("----------------------------------------\n");

	for (i = 0;i < 20;i ++) {

		rbtree_node *node = rbtree_search(T, keyArray[i]);
		rbtree_node *cur = rbtree_delete(T, node);
		free(cur);

		rbtree_traversal(T, T->root);
		printf("----------------------------------------\n");
	}
	

	
}





