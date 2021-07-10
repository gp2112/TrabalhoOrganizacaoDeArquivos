#ifndef ARVOREB_H
#define ARVOREB_H

typedef struct btree_ BTREE;

#include "dados.h"

#ifndef ORDEM
	#define ORDEM 5
#endif

#define NO_PROMOTION 0
#define PROMOTION 1


INDEX_HEADER *header_index_create();
INDEX_REG *create_indexreg(int rrn);
int64 btree_search(char *fname, int rrn, int key);
int btree_insert(char *fname, INDEX_HEADER *header, int rrn, int key, int pos, int *promo_child, int *promo_pos, int *promo_key);
void btree_print(char *fname, int rrn);
void print_node(INDEX_REG *node);

#endif