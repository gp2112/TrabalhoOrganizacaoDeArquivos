#ifndef ARVOREB_H
#define ARVOREB_H

typedef struct btree_ BTREE;


#ifndef ORDEM
	#define ORDEM 5
#endif

BTREE *btree_create();
int btree_search(BTREE *btree, int key);
void btree_insert(BTREE *btree, int key);
void btree_delete(BTREE **btree);

#endif