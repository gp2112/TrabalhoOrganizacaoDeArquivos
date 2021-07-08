#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"

INDEX_HEADER *header_index_create() {
	INDEX_HEADER *index = (INDEX_HEADER *)malloc(sizeof(INDEX_HEADER));

	if (index != NULL) {
		index->status = '0';
		index->noRaiz = -1;
		index->RRNproxNo = 1;
	}

	return index;
}

INDEX_REG *create_indexreg(int rrn) {

	INDEX_REG *index_reg = (INDEX_REG *)malloc(sizeof(INDEX_REG));
	if (index_reg == NULL)
		return NULL;


	for (int i=0; i<ORDEM-1; i++) {
		index_reg->cs[i] = -1;
		index_reg->ps[i] = -1;
		index_reg->prs[i] = -1;
	}


	index_reg->nroChavesIndexadas = 0;
	index_reg->RRNdoNo = rrn;
	index_reg->folha = '0';

	return index_reg;
}


// faz busca binária e retorna o índice do item (ou o do próximo caos n exista)
int binary_search(int *list, int key, int start, int end) {
	if (start == end)
		return end;

	int i = (int) ((start+end)/2.0);
	
	if (key == list[i])
		return i;
	if (key < list[i])
		return binary_search(list, key, start, i);
	if (key > list[i])
		return binary_search(list, key, i+1, end);
}


// retorna o byteoffset da chave ou retorna -1 caso não ache
int64 btree_search(FILE *f, int rrn, int key) {
	if (rrn == -1)
		return -1;

	INDEX_REG *node = bin_get_index_reg(f, rrn);

	int i = binary_search(node->keys, key, 0, node->nroChavesIndexadas);

	if (node->keys[i] == key) {
		int64 r = node->pos[i];
		free(node);
		return r;
	}

	int next = node->children[i];
	free(node);

	return btree_search(f, next, key);
}



void split(INDEX_REG *node, INDEX_REG *new_node, int key, int key_pos, int *promo_key, int *promo_pos, int *promo_child) {
	
	int keys[ORDEM], pos[ORDEM], children[ORDEM+1];

	for (int i=0; i<ORDEM; i++) {
		if (i<ORDEM-1) {
			keys[i] = node->keys[i];
			pos[i] = node->pos[i];
			
			// "Apaga" os valores
			node->keys[i] = -1;
			node->pos[i] = -1;
		}
		children[i] = node->children[i];
	}

	int index = binary_search(keys, key, 0, ORDEM-1);
	
	for (int i=ORDEM-1; i>index; i--) {
		keys[i] = keys[i-1];
		pos[i] = pos[i-1];
	}
	keys[index] = key;
	pos[index] = key_pos;

	escreve_index_data(f, new_node);

	*promo_key = keys[(int)(ORDEM/2)]; // chave a esquerda da direita
	*promo_pos = pos[(int)(ORDEM/2)];
	*promo_child = new_node->RRNdoNo;

	for (int i=0; i<(int)(ORDEM/2); i++) {
		node->keys[i] = keys[i];
		node->pos[i] = pos[i];
		new_node->keys[i] = keys[(int)(ORDEM/2)+1+i];
	}

}	


int btree_insert(FILE *f, INDEX_HEADER *header, int key, int pos, int *promo_child, int *promo_pos, int *promo_key) {
	if (rrn == -1) {
		*promo_key = key;
		*promo_pos = pos;
		*promo_child = -1;
		return PROMOTION;
	}

	// le a pagina
	INDEX_REG *node = bin_get_index_reg(f, rrn);

	int i = binary_search(node->keys, key, 0, node->nroChavesIndexadas);

	int r = btree_insert(f, node->children[i], header, key, pos, promo_child, promo_pos, promo_key);

	if (r == NO_PROMOTION || r == ERROR) {
		free(node);
		return r;
	}

	if (node->nroChavesIndexadas < ORDEM-1) {
		i = binary_search(node->keys, *promo_key, 0, node->nroChavesIndexadas);

		for (int j=node->nroChavesIndexadas-1; j>i; j--) {
			node->keys[j] = node->keys[j-1];
			node->pos[j] = node->pos[j-1];
		}
		node->keys[i] = *promo_key;
		node->pos[i] = *promo_pos;
		free(node);
		return NO_PROMOTION;
	}

	INDEX_REG *new_node = create_indexreg(prox_rrn);

	split(node, new_node, key, key_pos, promo_key, promo_pos);
	header->RRNproxNo++;

	escreve_index_data(f, node);
	escreve_index_data(f, new_node);

	free(node); free(new_node);

	return PROMOTION;
}


