#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"



typedef struct node_ NODE;


struct node_ {
	int size; // uso do nó max=ordem-1
	int rrm;
	int keys[ORDEM-1];
	NODE *children[ORDEM];
};

struct btree_ {
	NODE *root;
};


NODE *node_create() {
	NODE *node = (NODE *)malloc(sizeof(NODE));
	if (node != NULL) {
		node->rrm = -1;
		node->size = 0;
		for (int i=0; i<ORDEM; i++)
			node->children[i] = NULL;
	}
	return node;
}

BTREE *btree_create() {
	BTREE *btree = (BTREE *)malloc(sizeof(BTREE));
	if (btree != NULL)
		btree->root = node_create();
	
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

int node_search(NODE *root, int key) {
	if (root == NULL) return -1;

	int index = binary_search(root->keys, key, 0, root->size);
	if (root->keys[index] == key)
		return root->rrm;

	// se o índice retornado na BB não corresponde ao índice da chave
	// ela não existe. Assim é feita a busca na árvore filha
	return node_search(root->children[index], key);

	// se não tiver chave maior na página, vai pro último filho
	return node_search(root->children[ORDEM-1], key);
}

int btree_search(BTREE *btree, int key) {
	if (btree == NULL)
		return -1;

	return node_search(btree->root, key);
}




void split12(NODE *root) {
	NODE *node = node_create();
	node->rrm = root->rrm;

}

void node_insert_recursive(NODE *root, int key) {
	if (root == NULL) return ;


	if (root->size < ORDEM-1) {
		int i=0; root->size++;
		while (i < root->size && key < root->keys[i++]);

		for (int j=root->size-1; j>i; j--)
			root->keys[j] = root->keys[j-1];

		root->keys[i] = key; 

	}
}
/*
void btree_insert(BTREE *btree, int key) {



}*/


void delete_nodes(NODE **root) {
	if (*root==NULL) return;
	for (int i=0; i<ORDEM; i++)
		delete_nodes(&((*root)->children[i]));
	free(*root);
	*root = NULL;
}

void btree_delete(BTREE **btree) {
	if (*btree == NULL) return ;
	delete_nodes(&(*btree)->root);
	free(*btree);
	*btree = NULL;
}

