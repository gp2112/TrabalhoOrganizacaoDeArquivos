#ifndef ARVOREB_H
#define ARVOREB_H

typedef struct btree_ BTREE;

#include "dados.h"

#ifndef ORDEM
	#define ORDEM 5
#endif

#define NO_PROMOTION 0
#define PROMOTION 1
#define ERROR 2


INDEX_HEADER *header_index_create();
INDEX_REG *create_indexreg(int rrn);
int64 btree_search(FILE *f, int rrn, int key);
int btree_insert(FILE *f, INDEX_HEADER *header, int key, int pos, int *promo_child, int *promo_pos, int *promo_key);

#endif