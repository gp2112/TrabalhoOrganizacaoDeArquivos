#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreb.h"
#include "operacoes.h"
#include "util.h"

ERROR operation1(char *csv_fname, char *bin_fname) {
	FILE *csv_f=fopen(csv_fname, "r"), *bin_f=fopen(bin_fname, "wb");

	if (bin_f == NULL || csv_f == NULL) {
		if (bin_f != NULL) fclose(bin_f);
		if (csv_f != NULL) fclose(csv_f); 
		return FILE_ERROR;
	}

	VEICULO *veiculo = NULL;

	VEICULO_HEADER *header = header_veiculo_create(csv_f);

	escreve_header_veiculo(bin_f, header);
	while ((veiculo=get_veiculo(csv_f)) != NULL) {
		escreve_veiculo(bin_f, header, veiculo);
		veiculo_delete(&veiculo);
	}

	header_veiculo_alter_status(bin_f, '1');
	free(header);
	fclose(csv_f);
	fclose(bin_f);
	binarioNaTela(bin_fname);
	return 0;
}

ERROR operation2(char *csv_fname, char *bin_fname) {
	FILE *csv_f=fopen(csv_fname, "r"), *bin_f=fopen(bin_fname, "wb");

	if (bin_f == NULL || csv_f == NULL) {
		if (bin_f != NULL) fclose(bin_f);
		if (csv_f != NULL) fclose(csv_f); 
		return FILE_ERROR;
	}

	LINHA *linha = NULL;

	LINHA_HEADER *header = header_linha_create(csv_f);
	escreve_header_linha(bin_f, header);
	while ((linha=get_linha(csv_f)) != NULL) {
		escreve_linha(bin_f, header, linha);
		linha_delete(&linha);
	}

	header_linha_alter_status(bin_f, '1');
	free(header);
	fclose(csv_f);
	fclose(bin_f);
	binarioNaTela(bin_fname);
	return 0;
}

ERROR operation3(char *bin_fname) {
	FILE *bin_f=fopen(bin_fname, "rb");

	if (bin_f == NULL)
		return FILE_ERROR;

	// se o status do arquivo for 0, retorna erro
	VEICULO_HEADER *header = bin_get_header_veiculo(bin_f);
	if (header->status == '0') {
		fclose(bin_f); free(header);
		return FILE_ERROR;
	}

	VEICULO *veiculo = bin_get_veiculo(bin_f, NULL, NULL);

	if (veiculo==NULL) {
		fclose(bin_f);
		return REG_NULL;
	}


	do {
		if (veiculo->removido == '1')
			print_veiculo(veiculo);
		veiculo_delete(&veiculo);
	} while ((veiculo=bin_get_veiculo(bin_f, NULL, NULL)) != NULL);

	free(header);
	fclose(bin_f);
	return 0;
}

ERROR operation4(char *bin_fname) {
	FILE *bin_f=fopen(bin_fname, "rb");
	
	if (bin_f == NULL)
		return FILE_ERROR;

	// se o status do arquivo for 0, retorna erro
	LINHA_HEADER *header = bin_get_header_linha(bin_f);
	if (header->status == '0') {
		fclose(bin_f); free(header);
		return FILE_ERROR;
	}

	LINHA *linha = bin_get_linha(bin_f, NULL, NULL);

	if (linha==NULL) {
		fclose(bin_f);
		return REG_NULL;
	}

	do {
		if (linha->removido == '1')
			print_linha(linha);
		linha_delete(&linha);
	} while ((linha=bin_get_linha(bin_f, NULL, NULL)) != NULL);

	free(header);
	fclose(bin_f);
	return 0;
}

ERROR operation5(char *bin_fname, char *param, char *value) {
	FILE *bin_f = fopen(bin_fname, "rb");

	if (bin_f == NULL)
		return FILE_ERROR;

	// se o status do arquivo for 0, retorna erro
	VEICULO_HEADER *header = bin_get_header_veiculo(bin_f);
	if (header->status == '0') {
		fclose(bin_f); free(header);
		return FILE_ERROR;
	}

	VEICULO *veiculo = bin_get_veiculo(bin_f, param, value);
	if (veiculo == NULL) {
		fclose(bin_f);
		return REG_NULL;
	}

	do {
		print_veiculo(veiculo);
		veiculo_delete(&veiculo);

	} while ((veiculo=bin_get_veiculo(bin_f, param, value))!=NULL);

	free(header);
	fclose(bin_f);
	return 0;
}

ERROR operation6(char *bin_fname, char *param, char *value) {
	FILE *bin_f = fopen(bin_fname, "rb");

	if (bin_f == NULL)
		return FILE_ERROR;

	// se o status do arquivo for 0, retorna erro
	LINHA_HEADER *header = bin_get_header_linha(bin_f);
	if (header->status == '0') {
		fclose(bin_f); free(header);
		return FILE_ERROR;
	}

	LINHA *linha = bin_get_linha(bin_f, param, value);
	if (linha == NULL) {
		fclose(bin_f);
		return REG_NULL;
	}

	do {
		print_linha(linha);
		linha_delete(&linha);

	} while ((linha=bin_get_linha(bin_f, param, value))!=NULL);

	free(header);
	fclose(bin_f);
	return 0;
}

ERROR operation7(char *bin_fname, int n) {
	FILE *bin_f = fopen(bin_fname, "rb+");

	if (bin_f == NULL)
		return FILE_ERROR;

	VEICULO_HEADER *header = bin_get_header_veiculo(bin_f);
	if (header->status == '0') {
		fclose(bin_f); free(header);
		return FILE_ERROR;
	}

	char temp[100];

	header->status = '0';
	header_linha_alter_status(bin_f, '0');

	VEICULO *veiculo = NULL;

	for (int i=0; i<n; i++) {
		veiculo = (VEICULO *)malloc(sizeof(VEICULO));

		
		scan_quote_string(veiculo->prefixo);
		if (strcmp(veiculo->prefixo, "NULO")==0)
			preenche_lixo(veiculo->prefixo, 0, 5);
		else
			preenche_lixo(veiculo->prefixo, strlen(veiculo->prefixo), 5);
		

		scan_quote_string(veiculo->data);
		if (strcmp(veiculo->data, "NULO")==0)
			preenche_lixo(veiculo->data, 0, 11);
		else
			preenche_lixo(veiculo->data, strlen(veiculo->data), 11);

		scanf("%d %d", &veiculo->quantidadeLugares, &veiculo->codLinha);

		scan_quote_string(temp);
		if (strcmp(temp, "NULO")==0) {
			veiculo->tamanhoModelo = 0;
			veiculo->modelo = NULL;
		}
		else {
			veiculo->tamanhoModelo = strlen(temp);
			veiculo->modelo = (char *)calloc(veiculo->tamanhoModelo+1, sizeof(char));
			strcpy(veiculo->modelo, temp);
		}

		scan_quote_string(temp);
		if (strcmp(temp, "NULO")==0) {
			veiculo->tamanhoCategoria = 0;
			veiculo->categoria = NULL;
		}
		else {
			veiculo->tamanhoCategoria = strlen(temp);
			veiculo->categoria = (char *)calloc(veiculo->tamanhoCategoria+1, sizeof(char));
			strcpy(veiculo->categoria, temp);
		}

		veiculo->tamanhoRegistro = 31 + veiculo->tamanhoCategoria + veiculo->tamanhoModelo;
		veiculo->removido = '1';

		escreve_veiculo(bin_f, header, veiculo);
		veiculo_delete(&veiculo);
	}

	header_veiculo_alter_status(bin_f, '1');
	fclose(bin_f);
	free(header);
	binarioNaTela(bin_fname);
	return 0;
}

ERROR operation8(char *bin_fname, int n) {

	// abre o arquivo para ler o header
	FILE *bin_f = fopen(bin_fname, "rb+");

	if (bin_f == NULL)
		return FILE_ERROR;

	char temp[100];

	// se o status do arquivo for 0, retorna erro
	LINHA_HEADER *header = bin_get_header_linha(bin_f);
	if (header->status == '0') {
		fclose(bin_f);
		free(header);
		return FILE_ERROR;
	}


	header->status = '0';
	header_linha_alter_status(bin_f, '0');

	LINHA *linha = NULL;

	for (int i=0; i<n; i++) {

		linha = (LINHA *)malloc(sizeof(LINHA));
		
		scanf("%d", &linha->codLinha);

		scan_quote_string(&linha->aceitaCartao);

		scan_quote_string(temp);
		if (strcmp(temp, "NULO") == 0) {
			linha->nomeLinha = NULL;
			linha->tamanhoNome = 0;
		
		} else {
			linha->tamanhoNome = strlen(temp);
			linha->nomeLinha = (char*)calloc(linha->tamanhoNome+1, sizeof(char));
			strcpy(linha->nomeLinha, temp);
		}

		scan_quote_string(temp);
		if (strcmp(temp, "NULO") == 0) {
			linha->corLinha = NULL;
			linha->tamanhoCor = 0;
		}
		else {
			linha->tamanhoCor = strlen(temp);
			linha->corLinha = (char*)calloc(linha->tamanhoCor+1, sizeof(char));
			strcpy(linha->corLinha, temp);
		}
		linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;
		linha->removido = '1';
		
		escreve_linha(bin_f, header, linha);
		linha_delete(&linha);
	}

	header->status = '1';
	header_linha_alter_status(bin_f, '1');

	free(header);
	fclose(bin_f);

	binarioNaTela(bin_fname);

	return 0;
}


ERROR operation9(char *bin_fname, char *bin_index) {
	FILE *bin_data = fopen(bin_fname, "rb"), *index_f = fopen(bin_index, "wb");

	if (bin_data==NULL || index_f == NULL)
		return FILE_ERROR;

	// cria arquivo de índice e escreve header
	INDEX_HEADER *index_header = header_index_create();
	escreve_header_index(index_f, index_header);

	
	VEICULO_HEADER *header_veiculo = bin_get_header_veiculo(bin_data);

	VEICULO *veiculo = NULL; 

	// cria raiz
	INDEX_REG *root = create_indexreg(1);
	root->folha = '1';
	escreve_index_data(index_f, root);
	free(root); root=NULL;
	////////////////////

	fclose(index_f);

	// parametros para funcão de inserção (não precisam estar com valores atribuidos)
	int promo_key, promo_child, promo_pos, insert_return=-1;

	for (int i=0; i<header_veiculo->nroRegRemovidos; i++) {
		veiculo = bin_get_veiculo(bin_data, NULL, NULL); //pega o proximo veiculo

		if (veiculo->removido == '1') {
			printf("===================Nova Inserção==================%d\n\n", convertePrefixo(veiculo->prefixo));
			insert_return = btree_insert(bin_index, index_header, index_header->noRaiz, convertePrefixo(veiculo->prefixo), ftell(bin_data)-veiculo->tamanhoRegistro, &promo_child, &promo_pos, &promo_key);
		}

		if (insert_return == PROMOTION) {

			INDEX_REG *new_root = create_indexreg(++index_header->RRNproxNo);
			new_root->nroChavesIndexadas++;
			new_root->keys[0] = promo_key;
			new_root->pos[0] = promo_pos;
			new_root->children[0] = index_header->noRaiz;
			new_root->children[1] = promo_child;
			index_header->noRaiz = new_root->RRNdoNo;
			//index_header->RRNproxNo++;

			index_f = fopen(bin_index, "rb+");
			escreve_index_data(index_f, new_root);
			fclose(index_f);
			printf("NEW ROOT\n");
			print_node(new_root);
			free(new_root);
		}

		veiculo_delete(&veiculo);

	}

	/// DEBUG ///////
	//index_f = fopen(bin_index, "rb");
	//btree_print(index_f, index_header->noRaiz);
	//fclose(index_f);
	//////////////////


	index_f = fopen(bin_index, "rb+");
	index_header->status = '1';
	escreve_header_index(index_f, index_header);
	free(root); free(index_header);
	free(header_veiculo);
	fclose(bin_data);
	fclose(index_f);
	binarioNaTela(bin_index);
	return 0;
}
/*
ERROR operation10(char *bin_fname, char *bin_index) {
	FILE *bin_data = fopen(bin_fname, "rb"), *index_f = fopen(bin_index, "wb");

	if (bin_data==NULL || index_f == NULl)
		return FILE_ERROR;

	// cria arquivo de índice e escreve header
	INDEX_HEADER *index_header = header_index_create();
	escreve_header_index(index_f, index_header);


	BTREE *btree = btree_create();
	
	LINHA_HEADER *header_linha = bin_get_header_linha(bin_data);

	LINHA *linha = NULL;
	INDEX_REG *index_reg = NULL;

	for (int i=0; i<header_linha->nroRegRemovidos; i++) {
		linha = bin_get_linha(bin_fname, NULL, NULL); //pega a proxima linha
		
		if (linha->removido == '1')
			btree_insert(btree, linha->codLinha, ftell(bin_data)-linha->tamanhoRegistro);

		linha_delete(&linha);

	}

	btree_delete(&btree);
	fclose(bin_data);
	fclose(index_f);
	return 0;
}*/