#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"
#include "binparse.h"
#include "binout.h"

#define DEBUG 0
#define IS_D if(DEBUG)

INDEX_HEADER *header_index_create() {
	INDEX_HEADER *index = (INDEX_HEADER *)malloc(sizeof(INDEX_HEADER));

	if (index != NULL) {
		index->status = '0';
		index->noRaiz = -1;
		index->RRNproxNo = 0;
	}

	return index;
}

INDEX_REG *create_indexreg(int rrn) {

	INDEX_REG *index_reg = (INDEX_REG *)malloc(sizeof(INDEX_REG));
	if (index_reg == NULL)
		return NULL;


	for (int i=0; i<ORDEM-1; i++) {
		index_reg->keys[i] = -1;
		index_reg->children[i] = -1;
		index_reg->pos[i] = -1;
	}
	index_reg->children[ORDEM-1] = -1;

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




// imprime um nó
void print_node(INDEX_REG *node) {
	if (node==NULL) {
		printf("NO NULO!!!!!\n");
		return;
	}
	printf("\n===== NODE =====\n");
	printf("folha: %c\n", node->folha);
	printf("nroChavesIndexadas: %d\n", node->nroChavesIndexadas);
	printf("rrndoNo: %d\n", node->RRNdoNo);
	printf("CHAVES: ");
	for (int i=0; i<ORDEM-1; i++)
		printf("%d ", node->keys[i]); printf("\n");
	printf("POSICOES: ");
	for (int i=0; i<ORDEM-1; i++)
		printf("%lld ", node->pos[i]); printf("\n");
	printf("FILHOS: ");
	for (int i=0; i<ORDEM; i++)
		printf("%d ", node->children[i]); printf("\n");

	printf("\n");

}
// retorna o byteoffset da chave ou retorna -1 caso não ache
int64 btree_search(char *fname, int rrn, int key) {
	if (rrn == -1)
		return -1;

	INDEX_REG *node = bin_get_index_reg(fname, rrn);

	int i = binary_search(node->keys, key, 0, node->nroChavesIndexadas);
	print_node(node);

	if (node->keys[i] == key) {
		int64 r = node->pos[i];
		free(node);
		return r;
	}
	int next = node->children[i];
	free(node);

	return btree_search(fname, next, key);
}

// percorre a arvore para imprimir os nos 
void btree_print(char *fname, int rrn) {

	INDEX_REG *node = bin_get_index_reg(fname, rrn);
	if (node==NULL)
		return;

	print_node(node);
	btree_print(fname, rrn+1);
	/*
	for (int i=0; i<ORDEM; i++) {
		if (node->children[i] != -1)
			btree_print(fname, node->children[i]);
	}*/


	free(node);
}


void split(char *fname, INDEX_REG *node, INDEX_REG *new_node, int *promo_key, int *promo_pos, int *promo_child) {
	IS_D printf("\n======SPLIT!========\n");
	int keys[ORDEM], pos[ORDEM], children[ORDEM+1];

	IS_D printf("promokey antes: %d\n", *promo_key);

	for (int i=0; i<ORDEM; i++) {
		if (i<ORDEM-1) {
			keys[i] = node->keys[i];
			pos[i] = node->pos[i];
			
			// "Apaga" os valores
			node->keys[i] = -1;
			node->pos[i] = -1;
		}
		children[i] = node->children[i];
		node->children[i] = -1;
	}

	// acha a posicao da chave
	int index = binary_search(keys, *promo_key, 0, ORDEM-1);
	
	// inserção
	for (int i=ORDEM; i>index; i--) {
		if (i<ORDEM) {
			keys[i] = keys[i-1];
			pos[i] = pos[i-1];
		}
		children[i] = children[i-1];
	}

	IS_D printf("vetor filhos:\n");
	IS_D for (int i=0; i<=ORDEM; i++)
		printf("%d ", children[i]);
	IS_D printf("\n");

	// aqui
	keys[index] = *promo_key;
	pos[index] = *promo_pos;

	children[index+1] = *promo_child;

	IS_D printf("vetor filhos depois:\n");
	IS_D for (int i=0; i<=ORDEM; i++)
		printf("%d ", children[i]);
	IS_D printf("\n");

	*promo_key = keys[(int)(ORDEM/2)]; // chave a esquerda da direita
	*promo_pos = pos[(int)(ORDEM/2)];
	*promo_child = new_node->RRNdoNo;

	for (int i=0; i<(int)(ORDEM/2); i++) {
		node->keys[i] = keys[i];
		node->pos[i] = pos[i];
		node->children[i] = children[i];
		new_node->keys[i] = keys[(int)(ORDEM/2)+1+i];
		new_node->pos[i] = pos[(int)(ORDEM/2)+1+i];
		new_node->children[i] = children[(int)(ORDEM/2)+i+1];
	}
	node->children[(int)(ORDEM/2)] = children[(int)(ORDEM/2)];
	new_node->children[(int)(ORDEM/2)] = children[ORDEM];
	
	node->nroChavesIndexadas = (int)(ORDEM/2);
	new_node->nroChavesIndexadas = (int)(ORDEM/2);

	if (new_node->children[0]==-1)
		new_node->folha = '1';
}	

int btree_insert(char *fname, INDEX_HEADER *header, int rrn, int key, int pos, int *promo_child, int *promo_pos, int *promo_key) {

	if (rrn == -1) {
		IS_D printf("mudei -1\n");
		*promo_key = key;
		*promo_pos = pos;
		*promo_child = -1;
		return PROMOTION;
	}

	// le a pagina
	
	IS_D printf("RRN %d\n", rrn);
	INDEX_REG *node = bin_get_index_reg(fname, rrn);
	
	
	IS_D print_node(node);

	int i = binary_search(node->keys, key, 0, node->nroChavesIndexadas);

	int r = btree_insert(fname, header, node->children[i], key, pos, promo_child, promo_pos, promo_key);
	
	IS_D printf("Entrada promokey: %d\n", *promo_child);

	if (r == NO_PROMOTION) {
		free(node);
		return r;
	}

	if (node->nroChavesIndexadas < ORDEM-1) {
		IS_D printf("É o PAI\n");
		
		i = binary_search(node->keys, *promo_key, 0, node->nroChavesIndexadas);

		for (int j=ORDEM-1; j>i; j--) {
			if (j<=node->nroChavesIndexadas) {
				node->keys[j] = node->keys[j-1];
				node->pos[j] = node->pos[j-1];
			}
			node->children[j] = node->children[j-1];
		}
		node->keys[i] = *promo_key;
		node->pos[i] = *promo_pos;
		node->nroChavesIndexadas++;
		
		if (*promo_child != -1) {
			node->children[i+1] = *promo_child;
			node->folha = '0';
		}

		IS_D print_node(node);
		escreve_index_data(fname, node);
		free(node);
		return NO_PROMOTION;
	}


	INDEX_REG *new_node = create_indexreg(header->RRNproxNo);

	split(fname, node, new_node, promo_key, promo_pos, promo_child);

	IS_D printf("Promo child: %d", *promo_child);
	IS_D printf("\n");
	IS_D print_node(node);
	IS_D print_node(new_node); 
	IS_D printf("\n");

	escreve_index_data(fname, node);
	escreve_index_data(fname, new_node);
	header->RRNproxNo++;

	free(node); free(new_node);

	return PROMOTION;
}


