#ifndef __NM__BINTREE__H__
#define __NM__BINTREE__H__
#include "nmaux.h"
#include "nmlist.h"

typedef struct nmbintree_node_s nmbintree_node;
typedef struct nmbintree_s nmbintree;

nmbintree *nmbintree_alloc(void (*destructor)(void *data),
                           int (*cmp)(const void *e1, const void *e2));
						   
int nmbintree_free(nmbintree *tree, nm_free_mode mode);

int nmbintree_add_left(nmbintree *tree, nmbintree_node *treenode,
                       const void *data);
					   
int nmbintree_add_right(nmbintree *tree, nmbintree_node *treenode,
                        const void *data);
						
int nmbintree_purge_left(nmbintree *tree, nmbintree_node *treenode,
                         nm_free_mode mode);
						 
int nmbintree_purge_right(nmbintree *tree, nmbintree_node *treenode,
                          nm_free_mode mode);
						  
nmbintree *nmbintree_merge(nmbintree *leftree, nmbintree *rightree,
                           void (*destructor)(void *ddata),
                           int (*cmp)(const void *e1, const void *e2),
                           const void *data);
						   
unsigned int nmbintree_size(nmbintree *tree);

nmbintree_node *nmbintree_root(nmbintree *tree);

nmbintree_node *nmbintree_left(nmbintree_node *treenode);

nmbintree_node *nmbintree_right(nmbintree_node *treenode);

void *nmbintree_get_data(nmbintree_node *treenode);

void *nmbintree_get_root(nmbintree *tree);

void *nmbintree_get_left(nmbintree_node *treenode);

void *nmbintree_get_right(nmbintree_node *treenode);

int nmbintree_set_data(nmbintree_node *node, const void *data);

int nmbintree_set_root(nmbintree *tree, const void *data);

int nmbintree_set_left(nmbintree_node *node, const void *data);

int nmbintree_set_right(nmbintree_node *node, const void *data);

int nmbintree_preoder(nmbintree_node *node, nmlist *list);

int nmbintree_inorder(nmbintree_node *node, nmlist *list);

int nmmbintree_postorder(nmbintree_node *node, nmlist *list);

#endif
