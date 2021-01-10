#include <stdlib.h>
#include "nmbintree.h"

struct nmbintree_node_s {
	void *data;
	struct nmbintree_node_s *right;
	struct nmbintree_node_s *left;
};

struct nmbintree_s {
	unsigned int size;
	int (*cmp)(const void *e1, const void *e2);
	void (*destructor)(void *data);
	nmbintree_node *root;
};

unsigned int nmbintree_purge(nmbintree_node *treenode,
                             void (*destructor)(void *data), nm_free_mode mode);

/**
 * Allocates memory for a new binary tree.
 *
 * INPUT:
 * 'destructor'			Used to free data being held by nmbintree_node->data.
 * 'cmp'				Function used to compare to binary tree elements.
 *
 * RETURNS
 * NULL					If memory allocation fails.
 * A new binary tree.
 *
 **/
nmbintree *nmbintree_alloc(void (*destructor)(void *data),
                           int (*cmp)(const void *e1, const void *e2))
{
	nmbintree* tree = NULL;
	if ((tree = malloc(sizeof(*tree))) != NULL) {
		tree->size = 0;
		tree->root = NULL;
		tree->destructor = destructor;
		tree->cmp = cmp;
	}
	return tree;
}

/**
 * Free data structure.
 * If tree is NULL, returns (-1).
 * If tree->destructor is NULL and mode is HARD, returns (-1).
 *
 * mode:
 *	SOFT		: Will free only consisting nodes, and the tree
 * 					structure. 'data' being held will be preserved.
 *  HARD		: Will free consisting nodes, the 'tree', and
 * 					the 'data' being held by the nodes.
 **/
int nmbintree_free(nmbintree *tree, nm_free_mode mode)
{
	if (tree == NULL) {
		return (-1);
	}
	if (tree->size != 0) {
		if (mode == HARD && tree->destructor == NULL){
			return (-1);
		}
		nmbintree_purge_left(tree, NULL, mode);
	}
	free(tree);
	return (0);
}

/**
 * Adds a new element to the left of 'treenode'.
 * If 'treenode' has a left child, returns (-1);
 * If 'treenode' is NULL, the new element is inserted as root ('tree' must be
 * not NULL, but empty).
 * If 'treenode' is NULL, and 'tree' is NULL, returns (-1).
 * If memory allocation fails for a new node, returns (-1).
 *
 * INPUT:
 * 'tree'			Binary tree where to insert new element.
 * 'treenode'		Where to insert the left child.
 * 'data'			Data being held by the new node.
 *
 * RETURNS:
 *	0				If insertion is succesful.
 * -1				If something went wrong.
 **/
int nmbintree_add_left(nmbintree *tree, nmbintree_node *treenode,
                       const void *data)
{
	nmbintree_node *new_node = NULL;
	nmbintree_node **where_to = NULL;
	if (tree == NULL) {
		return (-1);
	}
	if (treenode == NULL) {
		if (tree->size == 0) {
			return (-1);
		}
		where_to = &tree->root;
	} else {
		if (treenode->right != NULL) {
			return (-1);
		}
		where_to = &treenode->left;
	}
	new_node = malloc(sizeof(*new_node));
	if (new_node == NULL) {
		return (-1);
	}
	new_node->data = (void*) data;
	new_node->left = NULL;
	new_node->right = NULL;
	*where_to = new_node;
	tree->size++;
	return (0);
}

/**
 * Adds a new element to the right of 'treenode'.
 * If 'treenode' has a right child, returns (-1);
 * If 'treenode' is NULL, the new element is inserted as root ('tree' must be
 * not NULL, but empty).
 * If 'treenode' is NULL, and 'tree' is NULL, returns (-1).
 * If memory allocation fails for a new node, returns (-1).
 *
 * INPUT:
 * 'tree'			Binary tree where to insert new element.
 * 'treenode'		Where to insert the right child.
 * 'data'			Data being held by the new node.
 *
 * RETURNS:
 *	0				If insertion is succesful.
 * -1				If something went wrong.
 **/
int nmbintree_add_right(nmbintree *tree, nmbintree_node *treenode,
                        const void *data)
{
	nmbintree_node *new_node = NULL;
	nmbintree_node **where_to = NULL;
	if (tree == NULL) {
		return (-1);
	}
	if (treenode == NULL) {
		if (tree->size == 0) {
			return (-1);
		}
		where_to = &tree->root;
	} else {
		if (treenode->right != NULL) {
			return (-1);
		}
		where_to = &treenode->right;
	}
	new_node = malloc(sizeof(*new_node));
	if (new_node == NULL) {
		return (-1);
	}
	new_node->data = (void*) data;
	new_node->left = NULL;
	new_node->right = NULL;
	*where_to = new_node;
	tree->size++;
	return (0);
}

/**
 * THIS FUNCTION IS PRIVATE.
 * TO DO: INVESTIGATE A NON-RECURSIVE ALGORITHM.
 * Removes and de-allocate memory for all the nodes
 * bellow treenode (+treenode).
 *
 * All data being held by those node is
 * also being freed.
 *
 * If 'treenode' or 'destructor' are NULL,
 * the function returns (0).
 *
 * IF 'mode':
 * SOFT			: Will free only node_elements, data being held
 * 				by the node elements will be preserved.
 * HARD			: Will free also data.
 *
 * Returns:
 * The number of destroyed nodes.
 **/
unsigned int nmbintree_purge(nmbintree_node *treenode,
                             void (*destructor)(void *data), nm_free_mode mode)
{
	nmbintree_node *left, *right;
	if (treenode == NULL || destructor == NULL) {
		return (0);
	}
	left = treenode->left;
	right = treenode->right;
	free(treenode);
	if (mode == HARD) {
		destructor(treenode->data);
	}
	return (nmbintree_purge(left, destructor, mode) +
	        nmbintree_purge(right, destructor, mode) + 1);
}

/**
 * Removes and de-allocates memory for the the subtree
 * that has its root in treenode->left.
 *
 * If 'treenode' is NULL, the call will be equivalent with a nmbintree_free.
 * If 'tree->destructor' is NULL, returns (-1);
 * If 'tree' is NULL, returns (-1);
 *
 * The function can return (-1) if a inner memory allocation fails.
 **/
int nmbintree_purge_left(nmbintree *tree, nmbintree_node *treenode, nm_free_mode mode)
{
	nmbintree_node **start_node = NULL;
	nmbintree_node *tmp;
	if (tree == NULL || tree->destructor == NULL) {
		return (-1);
	}
	if (treenode == NULL) {
		if (tree == NULL) {
			return (-1);
		}
		start_node = &tree->root;
	} else {
		start_node = &treenode->left;
	}
	tree->size -= nmbintree_purge(*start_node, tree->destructor, mode);
	return (0);
}

/**
 * Removes and de-allocates memory for the the subtree
 * that has its root in treenode->right.
 *
 * If 'treenode' is NULL, the call will be equivalent with a nmbintree_free.
 * If 'tree->destructor' is NULL, returns (-1);
 * If 'tree' is NULL, returns (-1);
 *
 * The function can return (-1) if a inner memory allocation fails.
 **/
int nmbintree_purge_right(nmbintree *tree, nmbintree_node *treenode, nm_free_mode mode)
{
	nmbintree_node **start_node = NULL;
	nmbintree_node *tmp;
	if (tree == NULL || tree->destructor == NULL) {
		return (-1);
	}
	if (treenode == NULL) {
		if (tree == NULL) {
			return (-1);
		}
		start_node = &tree->root;
	} else {
		start_node = &treenode->right;
	}
	tree->size -= nmbintree_purge(*start_node, tree->destructor, mode);
	return (0);
}

/**
 * Merges two subtrees: 'leftree' and 'rightree' into 'tree'.
 * 'tree-root' will hold 'data'.
 *
 * If memory allocation fails, the function will return NULL.
 *
 * leftree && rightree shouldn't be NULL, else the function will
 * fail.
 *
 * If merge operation is succesful, the function will also perform
 * free on 'leftree' and 'rightree' (the data being held by the nodes,
 * and the nodes won't get affected).
 *
 * Take in mind that modifying an element in rightree/leftree, may also change
 * the behaviour in the resulting tree. As a best practice you should
 * apply a soft free on 'leftree' and 'rightree'.
 **/
nmbintree *nmbintree_merge(nmbintree *leftree, nmbintree *rightree,
                           void (*destructor)(void *ddata),
                           int (*cmp)(const void *e1, const void *e2),
                           const void *data)
{
	nmbintree *tree = NULL;
	if (leftree == NULL || rightree == NULL) {
		return NULL;
	}
	tree = nmbintree_alloc(destructor, cmp);
	if (tree != NULL) {
		return NULL;
	}
	if (nmbintree_add_left(tree, NULL, data) == -1) {
		free(tree);
		return NULL;
	}
	tree->size += (leftree->size + rightree->size);
	tree->root->left = leftree->root;
	tree->root->right = rightree->root;
	return tree;
}

/**
 * Returns tree->size;
 * 0 If tree is NULL or empty.
 **/
unsigned int nmbintree_size(nmbintree *tree)
{
	return (tree == NULL) ? 0 : tree->size;
}

/**
 * Returns tree->root;
 * NULL If tree is NULL or root is NULL.
 **/
nmbintree_node *nmbintree_root(nmbintree *tree)
{
	return (tree == NULL) ? NULL : tree->root;
}

/**
 * Returns treenode->left.
 * NULL if treenode is NULL, or treenode->left is NULL.
 **/
nmbintree_node *nmbintree_left(nmbintree_node *treenode)
{
	return (treenode == NULL) ? NULL : treenode->left;
}

/**
 * Returns treenode->right.
 * NULL if treenode is NULL, or treenode->right is NULL.
 **/
nmbintree_node *nmbintree_right(nmbintree_node *treenode)
{
	return (treenode == NULL) ? NULL : treenode->right;
}

/**
 * Returns treenode->data.
 * NULL if treenode->data or treenode is NULL.
 **/
void *nmbintree_get_data(nmbintree_node *treenode)
{
	return (treenode == NULL) ? NULL : treenode->data;
}

/**
 * Returns treenode->root->data.
 * NULL if tree is NULL, tree->root is NULL, tree->root->data is NULL.
 **/
void *nmbintree_get_root(nmbintree *tree)
{
	return (tree == NULL || tree->root == NULL) ?
	       NULL : tree->root->data;
}

/**
 * Returns treenode->left->data.
 * NULL if treenode is NULL, treenode->left is NULL,
 * treenode->left->data is NULL.
 **/
void *nmbintree_get_left(nmbintree_node *treenode)
{
	return (treenode == NULL || treenode->left == NULL) ?
	       NULL : treenode->left->data;
}

/**
 * Returns treenode->right->data.
 * NULL if treenode is NULL, treenode->right is NULL,
 * treenode->right->data is NULL.
 **/
void *nmbintree_get_right(nmbintree_node *treenode)
{
	return (treenode == NULL || treenode->right == NULL) ?
	       NULL : treenode->right->data;
}

/**
 * Updates data being held by node.
 * Returns:
 * 0		If operation is succesful.
 * -1		If operation fails (node is NULL).
 **/
int nmbintree_set_data(nmbintree_node *node, const void *data)
{
	if (node == NULL) {
		return (-1);
	}
	node->data = (void*) data;
	return (0);
}

/**
 * Updates data being held by root element
 * of the 'tree'.
 *
 * RETURNS:
 * 0		If operation is succesful.
 * -1		If operation fails (tree is NULL, tree->root is NULL).
 **/
int nmbintree_set_root(nmbintree *tree, const void *data)
{
	return (tree == NULL ) ?
	       (-1) : nmbintree_set_data(tree->root, data);
}

/**
 * Updates data being held by the left child
 * of node.
 *
 * RETURNS:
 * 0		If operation is succesful.
 * -1		If operation fails.
 **/
int nmbintree_set_left(nmbintree_node *node, const void *data)
{
	return (node == NULL) ? (-1) : nmbintree_set_data(node->left, data);
}

/**
 * Updates data being held by the right child
 * of node.
 *
 * RETURNS:
 * 0		If operation is succesful.
 * -1		If operation fails.
 **/
int nmbintree_set_right(nmbintree_node *node, const void *data)
{
	return (node == NULL) ? (-1) : nmbintree_set_data(node->right, data);
}
