#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"


struct btree_ {
	int size; // uso do nó max=ordem-1
	int rrn;
	int keys[ORDEM-1];
	int64 pos[ORDEM-1]; //respectivas posições das chaves
	BTREE *children[ORDEM];
};



BTREE *btree_create(int rrn) {
	BTREE *btree = (BTREE *)malloc(sizeof(BTREE));
	if (btree != NULL) {
		btree->rrn = rrn;
		for (int i=0; i<ORDEM-1; i++) {
			btree->keys[i] = -1;
			btree->pos[i] = -1;
			btree->children[i] = NULL;
		}
		btree->children[ORDEM-1] = NULL;
	}
	
	return btree;
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
		return binary_search(list, key, i, end);
}

int btree_recursive_search(BTREE *root, int key) {
	if (root == NULL) return -1;

	int index = binary_search(root->keys, key, 0, root->size);
	if (root->keys[index] == key)
		return root->rrn;

	// se o índice retornado na BB não corresponde ao índice da chave
	// ela não existe. Assim é feita a busca na árvore filha
	return btree_recursive_search(root->children[index], key);

	// se não tiver chave maior na página, vai pro último filho
	return btree_recursive_search(root->children[ORDEM-1], key);
}

int btree_search(BTREE *btree, int key) {
	if (btree == NULL)
		return -1;

	return btree_recursive_search(btree, key);
}




void split12(BTREE *root) {
	BTREE *subtree = btree_create();
	subtree->rrn = root->rrn;

}


// salva/atualiza o nó no arquivo de índice
void save_node(FILE *f, BTREE *btree) {
	fseek(f, 77*btree->rrn, SEEK_SET);

	INDEX_REG *index_reg = NULL;

	index_reg = bin_get_index_reg(f);

	// se já existe no arquivo
	if (index_reg == NULL) 
		index_reg = create_indexreg();

	for (int i=0; i<ORDEM-1; i++) {
		index_reg->cs[i] = btree->keys[i];
		index_reg->prs[i] = btree->pos[i];
		index_reg->ps[i] = btree->children[i]==NULL ? -1 : btree->children[i]->rrn;
	}
	index_reg->ps[ORDEM-1] = btree->children[ORDEM-1]==NULL ? -1 : btree->children[ORDEM-1]->rrn;

	index_reg->folha = btree->size==0 ? '1' : '0';
	index_reg->nroChavesIndexadas = btree->size;
	index_reg->RRNdoNo = btree->rrn;

	escreve_index_data(f, index_reg);
}

void node_insert_recursive(FILE *f, BTREE *root, int key) {
	if (root == NULL) return ;


	if (root->size < ORDEM-1) {
		int i=0; root->size++;
		while (i < root->size && key < root->keys[i++]);

		for (int j=root->size-1; j>i; j--)
			root->keys[j] = root->keys[j-1];

		root->keys[i] = key; 

	}
}

void btree_insert(FILE *f, BTREE *btree, int key, int pos) {



}


void btree_delete(BTREE **btree) {
	if (*root==NULL) return;
	for (int i=0; i<ORDEM; i++)
		recursive_delete(&((*root)->children[i]));
	free(*root);
	*root = NULL;
}

