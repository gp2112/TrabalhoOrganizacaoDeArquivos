#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"


struct btree_ {
	int size; // uso do nó max=ordem-1
	int rrn;
	int keys[ORDEM-1]; // ordenado | valores nulos = -1
	int64 pos[ORDEM-1]; //respectivas posições das chaves | valores nulos = -1
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
		return -1;
	int i = (int) ((start+end)/2.0);

	if (key == list[i])
		return i;
	if (key < list[i])
		return binary_search(list, key, start, i-1);
	if (key > list[i])
		return binary_search(list, key, i+1, end);
}

int btree_recursive_search(BTREE *root, int key) {
	if (root == NULL) return -1;

	int index = binary_search(root->keys, key, 0, root->size);
	if (root->keys[index] == key)
		return root;

	// se o índice retornado na BB não corresponde ao índice da chave
	// ela não existe. Assim é feita a busca na árvore filha
	return btree_recursive_search(root->children[index], key);

	// se não tiver chave maior na página, vai pro último filho
	return btree_recursive_search(root->children[ORDEM-1], key);
}

// retorna -1 caso não ache a chave
int64 btree_search(BTREE *btree, int key) {
	if (btree == NULL)
		return -1;

	BTREE *r = btree_recursive_search(btree, key);

	int i = binary_search(r->keys, key, 0, btree->size);

	if (i==-1)
		return -1;

	return r->pos[i];
}



void split12(BTREE *root) {
	//BTREE *subtree = btree_create();
	//subtree->rrn = root->rrn;
	return;
}

// verifica se nó é folha
// retorna 1 - verdadeiro ou 0 - Falso
char is_leaf(BTREE *btree) {
	return (btree->children[0]==NULL);
}


/*
Atualiza ou salva (caso o nó não exista ainda) o nó
no arquivo de índices.

*/
void save_node(FILE *f, BTREE *btree) {

	// vai para a posição do próximo nó
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

	index_reg->folha = '0'+is_leaf(btree);
	index_reg->nroChavesIndexadas = btree->size;
	index_reg->RRNdoNo = btree->rrn;

	escreve_index_data(f, index_reg);
}

void node_insert_recursive(FILE *f, BTREE *root, int key) {
	if (root == NULL) return ;

	int i=0; root->size++;
	while (i < root->size && key < root->keys[i++]);

	for (int j=root->size-1; j>i; j--)
		root->keys[j] = root->keys[j-1];

	root->keys[i] = key; 
	


	save_node(f, root);
}

void btree_insert(FILE *f, BTREE *btree, int key, int pos) {

	if (root == NULL) return ;

	if (root->size < ORDEM-1) {
		node_insert_recursive(f, btree, key);
		
	} else 
		split12(btree);


}


void btree_delete(BTREE **btree) {
	if (*root==NULL) return;
	for (int i=0; i<ORDEM; i++)
		recursive_delete(&((*root)->children[i]));
	free(*root);
	*root = NULL;
}

