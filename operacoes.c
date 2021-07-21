#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvoreb.h"
#include "operacoes.h"
#include "util.h"

#define DEBUG 0
#define IS_D if(DEBUG)

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


	header->status = '0';
	header_linha_alter_status(bin_f, '0');

	VEICULO *veiculo = NULL;

	for (int i=0; i<n; i++) {
		veiculo = veiculo_input();

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
		
		linha = linha_input();
		
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
	FILE *bin_data = fopen(bin_fname, "rb"), 
			*index_f = fopen(bin_index, "wb"); //cria arquivo de indice
	fclose(index_f);

	if (bin_data==NULL || index_f == NULL)
		return FILE_ERROR;

	VEICULO_HEADER *header_veiculo = bin_get_header_veiculo(bin_data);

	if (header_veiculo->status == '0') {
		free(header_veiculo);
		return FILE_ERROR;
	}

	// cria arquivo de índice e escreve header
	INDEX_HEADER *index_header = header_index_create();
	escreve_header_index(bin_index, index_header);

	

	VEICULO *veiculo = NULL; 

	// cria raiz
	INDEX_REG *root = create_indexreg(0);
	root->folha = '1';

	escreve_index_data(bin_index, root);

	index_header->RRNproxNo++;
	index_header->noRaiz = 0;
	free(root); root=NULL;
	////////////////////


	// parametros para funcão de inserção (não precisam estar com valores atribuidos)
	int promo_key, promo_child, promo_pos, insert_return=-1;
	
	int64 byteoffset = ftell(bin_data);

	// lê veiculos até o final do arquivo
	while ((veiculo = bin_get_veiculo(bin_data, NULL, NULL)) != NULL) {
		
		if (veiculo->removido == '1') {
			IS_D print_veiculo(veiculo);
			IS_D printf("===================Nova Inserção==================\ninsere: %d\n\n", convertePrefixo(veiculo->prefixo));

			insert_return = btree_insert(bin_index, index_header, index_header->noRaiz, convertePrefixo(veiculo->prefixo), byteoffset, &promo_child, &promo_pos, &promo_key);

			// verifica se houve promoção
			// se houve, haverá a criação de uma nova raiz
			if (insert_return == PROMOTION) {
				// cria novo nó raiz
				INDEX_REG *new_root = create_indexreg(index_header->RRNproxNo);

				// adiciona a chave/posicao e os filhos no novo nó
				new_root->nroChavesIndexadas++;
				new_root->keys[0] = promo_key;
				new_root->pos[0] = promo_pos;
				new_root->children[0] = index_header->noRaiz;
				new_root->children[1] = promo_child;

				// atualiza no header a nova raiz
				index_header->noRaiz = new_root->RRNdoNo;
				index_header->RRNproxNo++;
				
				// salva o nó em disco
				escreve_index_data(bin_index, new_root);
				
				IS_D printf("NEW ROOT\n");
				IS_D print_node(new_root);
				free(new_root);
			}
		}
		veiculo_delete(&veiculo);

		// salva a posição do próximo veículo
		byteoffset = ftell(bin_data);
	}

	index_header->status = '1';
	escreve_header_index(bin_index, index_header);
	fclose(bin_data);

	binarioNaTela(bin_index);

	/*=============== DEBUG ======================
	printf("\n\n=====IMPRIME ARVORE=====\n\n");
	btree_print(bin_index, index_header->noRaiz);
	//============================================*/
	
	free(root); free(index_header);
	free(header_veiculo);
	return 0;
}


ERROR operation10(char *bin_fname, char *bin_index) {
	FILE *bin_data = fopen(bin_fname, "rb"), 
			*index_f = fopen(bin_index, "wb"); //cria arquivo de indice
	fclose(index_f);

	if (bin_data==NULL)
		return FILE_ERROR;
	LINHA_HEADER *header_linha = bin_get_header_linha(bin_data);
	if (header_linha->status == '0') {
		free(header_linha);
		return FILE_ERROR;
	}

	// cria arquivo de índice e escreve header
	INDEX_HEADER *index_header = header_index_create();
	escreve_header_index(bin_index, index_header);

	

	LINHA *linha = NULL; 

	// cria raiz
	INDEX_REG *root = create_indexreg(0);
	root->folha = '1';
	escreve_index_data(bin_index, root);
	index_header->RRNproxNo++;
	index_header->noRaiz = 0;
	free(root); root=NULL;
	////////////////////


	// parametros para funcão de inserção (não precisam estar com valores atribuidos)
	int promo_key, promo_child, promo_pos, insert_return=-1;
	
	int64 byteoffset = ftell(bin_data);

	// lê linhas até o final do arquivo
	while ((linha = bin_get_linha(bin_data, NULL, NULL)) != NULL) {
		
		if (linha->removido == '1') {
			IS_D print_linha(linha);
			IS_D printf("===================Nova Inserção==================\ninsere: %d\n\n", linha->codLinha);

			insert_return = btree_insert(bin_index, index_header, index_header->noRaiz, linha->codLinha, byteoffset, &promo_child, &promo_pos, &promo_key);

			// verifica se houve promoção
			// se houve, haverá a criação de uma nova raiz
			if (insert_return == PROMOTION) {
				// cria novo nó raiz
				INDEX_REG *new_root = create_indexreg(index_header->RRNproxNo);

				// adiciona a chave/posicao e os filhos no novo nó
				new_root->nroChavesIndexadas++;
				new_root->keys[0] = promo_key;
				new_root->pos[0] = promo_pos;
				new_root->children[0] = index_header->noRaiz;
				new_root->children[1] = promo_child;

				// atualiza no header a nova raiz
				index_header->noRaiz = new_root->RRNdoNo;
				index_header->RRNproxNo++;
				
				// salva o nó em disco
				escreve_index_data(bin_index, new_root);
				
				IS_D printf("NEW ROOT\n");
				IS_D print_node(new_root);
				free(new_root);
			}
		}
		linha_delete(&linha);

		// salva a posição da próxima linha
		byteoffset = ftell(bin_data);
	}

	// salva o header atualizado
	index_header->status = '1';
	escreve_header_index(bin_index, index_header);
	fclose(bin_data);

	binarioNaTela(bin_index);

	/*=============== DEBUG ======================
	printf("\n\n=====IMPRIME ARVORE=====\n\n");
	btree_print(bin_index, index_header->noRaiz);
	//============================================*/
	
	free(root); free(index_header);
	free(header_linha);
	return 0;
}


ERROR operation11(char *bin_fname, char *bin_index, char *prefixo) {
	INDEX_HEADER *index_header = bin_get_header_index(bin_index);
	if (index_header==NULL)
		return FILE_ERROR;

	if (index_header->status == '0') {
		free(index_header);
		return FILE_ERROR;
	}
	FILE *veiculo_file = fopen(bin_fname, "rb");
	if (veiculo_file==NULL)
		return FILE_ERROR;

	VEICULO_HEADER *header_veiculo = bin_get_header_veiculo(veiculo_file);
	if (header_veiculo->status == '0') {
		fclose(veiculo_file);
		free(header_veiculo);
		free(index_header);
		return FILE_ERROR;
	}

	IS_D printf("procurando %d\n", convertePrefixo(prefixo));

	// caso encontre o veiculo, salva sua posição no arquivo de registro
	int64 veiculo_offset = btree_search(bin_index, index_header->noRaiz, convertePrefixo(prefixo));
	
	// se for -1, não foi encontrado veiculo
	if (veiculo_offset == -1) {
		free(index_header);
		return REG_NULL;
	}

	// vai para a posição encontrado no arquivo de registro e lê o veiculo
	fseek(veiculo_file, veiculo_offset, SEEK_SET);
	VEICULO *veiculo = bin_get_veiculo(veiculo_file, NULL, NULL);

	free(index_header);

	// imprime o veiculo encontrado
	print_veiculo(veiculo);
	veiculo_delete(&veiculo);
	fclose(veiculo_file);

	return 0;
}

ERROR operation12(char *bin_fname, char *bin_index, int codLinha) {
	INDEX_HEADER *index_header = bin_get_header_index(bin_index);
	if (index_header==NULL)
		return FILE_ERROR;

	FILE *linha_file = fopen(bin_fname, "rb");
	if (linha_file == NULL)
		return FILE_ERROR;

	LINHA_HEADER *header_linha = bin_get_header_linha(linha_file);
	if (header_linha->status == '0') {
		fclose(linha_file);
		free(index_header);
		return FILE_ERROR;
	}


	IS_D printf("procurando %d\n", codLinha);

	// caso encontre o veiculo, salva sua posição no arquivo de registro
	int64 linha_offset = btree_search(bin_index, index_header->noRaiz, codLinha);
	
	if (linha_offset == -1) {
		free(index_header);
		return REG_NULL;
	}

	if (linha_file == NULL) {
		free(index_header);
		return FILE_ERROR;
	}

	// vai para a posição encontrada no arquivo de registro
	fseek(linha_file, linha_offset, SEEK_SET);
	LINHA *linha = bin_get_linha(linha_file, NULL, NULL);

	free(index_header);

	//imprime a linha encontrada
	print_linha(linha);

	linha_delete(&linha);
	fclose(linha_file);

	return 0;
}

ERROR operation13(char *bin_fname, char *bin_index, int n) {
	FILE *veiculo_bin = fopen(bin_fname, "rb");

	if (veiculo_bin==NULL)
		return FILE_ERROR;

	INDEX_HEADER *index_header = bin_get_header_index(bin_index);

	if (index_header==NULL) {
		fclose(veiculo_bin);
		return FILE_ERROR;
	}

	VEICULO_HEADER *header_veiculo = bin_get_header_veiculo(veiculo_bin);


	if (header_veiculo->status=='0' || index_header->status=='0') {
		free(header_veiculo); free(index_header);
		return FILE_ERROR;
	}

	fclose(veiculo_bin);



	VEICULO *veiculo = NULL;
	int64 veiculo_offset;

	// parametros para funcão de inserção (não precisam estar com valores atribuidos)
	int promo_key, promo_child, promo_pos, insert_return=-1;

	for (int i=0; i<n; i++) {
		veiculo_bin = fopen(bin_fname, "rb+");
		// vai para o final do arquivo de registro
		fseek(veiculo_bin, 0, SEEK_END);

		// atribui a posição final ao byte offset do veiculo a ser inserido na arvore
		veiculo_offset = ftell(veiculo_bin);
		header_veiculo->byteProxReg = veiculo_offset;

		veiculo = veiculo_input();

		escreve_veiculo(veiculo_bin, header_veiculo, veiculo);

		insert_return = btree_insert(bin_index, index_header, index_header->noRaiz, convertePrefixo(veiculo->prefixo), veiculo_offset, &promo_key, &promo_child, &promo_pos);

		// verifica se houve promoção
			// se houve, haverá a criação de uma nova raiz
		if (insert_return == PROMOTION) {
				// cria novo nó raiz
			INDEX_REG *new_root = create_indexreg(index_header->RRNproxNo);

				// adiciona a chave/posicao e os filhos no novo nó
			new_root->nroChavesIndexadas++;
			new_root->keys[0] = promo_key;
			new_root->pos[0] = promo_pos;
			new_root->children[0] = index_header->noRaiz;
			new_root->children[1] = promo_child;

				// atualiza no header a nova raiz
			index_header->noRaiz = new_root->RRNdoNo;
			index_header->RRNproxNo++;
				
				// salva o nó em disco
			escreve_index_data(bin_index, new_root);
				
			IS_D printf("NEW ROOT\n");
			IS_D print_node(new_root);
			free(new_root);
		}

		veiculo_delete(&veiculo);
		fclose(veiculo_bin);
	}
	veiculo_bin = fopen(bin_fname, "rb+");

	// escreve o header atualizado em disco
	escreve_header_index(bin_index, index_header);
	header_veiculo_alter_status(veiculo_bin, '1');
	free(header_veiculo);
	fclose(veiculo_bin);
	binarioNaTela(bin_index);
	return 0;
}


ERROR operation14(char *bin_fname, char *bin_index, int n) {
	FILE *linha_bin = fopen(bin_fname, "rb");
	if (linha_bin==NULL)
		return FILE_ERROR;

	INDEX_HEADER *index_header = bin_get_header_index(bin_index);
	if (index_header==NULL) {
		fclose(linha_bin);
		return FILE_ERROR;
	}

	LINHA_HEADER *header_linha = bin_get_header_linha(linha_bin);
	fclose(linha_bin);

	if (header_linha->status=='0' || index_header->status=='0') {
		free(header_linha); free(index_header);
		return FILE_ERROR;
	}


	LINHA *linha = NULL;
	int64 linha_offset;

	// parametros para funcão de inserção (não precisam estar com valores atribuidos)
	int promo_key, promo_child, promo_pos, insert_return=-1;

	for (int i=0; i<n; i++) {	
		linha_bin = fopen(bin_fname, "rb+");

		// atribui a posição final ao byte offset do veiculo a ser inserido na arvore
		fseek(linha_bin, 0, SEEK_END);
		linha_offset = ftell(linha_bin);
		header_linha->byteProxReg = linha_offset;

		linha = linha_input();

		escreve_linha(linha_bin, header_linha, linha);

		insert_return = btree_insert(bin_index, index_header, index_header->noRaiz, linha->codLinha, linha_offset, &promo_key, &promo_child, &promo_pos);

		// verifica se houve promoção
			// se houve, haverá a criação de uma nova raiz
		if (insert_return == PROMOTION) {
				// cria novo nó raiz
			INDEX_REG *new_root = create_indexreg(index_header->RRNproxNo);

			// adiciona a chave/posicao e os filhos no novo nó
			new_root->nroChavesIndexadas++;
			new_root->keys[0] = promo_key;
			new_root->pos[0] = promo_pos;
			new_root->children[0] = index_header->noRaiz;
			new_root->children[1] = promo_child;

			// atualiza no header a nova raiz
			index_header->noRaiz = new_root->RRNdoNo;
			index_header->RRNproxNo++;
			
			// salva o nó em disco
			escreve_index_data(bin_index, new_root);
			
			IS_D printf("NEW ROOT\n");
			IS_D print_node(new_root);
			free(new_root);
		}

		linha_delete(&linha);
		fclose(linha_bin);
	}
	linha_bin = fopen(bin_fname, "rb+");
	escreve_header_index(bin_index, index_header);
	header_linha_alter_status(linha_bin, '1');
	free(header_linha);
	fclose(linha_bin);
	binarioNaTela(bin_index);
	return 0;
}


ERROR operation15(char *veic_f, char *linha_f) {
	FILE *veiculo_file = fopen(veic_f, "rb"),
		 *linha_file = fopen(linha_f, "rb");


	// verifica se arquivos existem
	if (veiculo_file == NULL) {
		if(linha_file!=NULL) fclose(linha_file);
		return FILE_ERROR;
	}
	if (linha_file == NULL) {
		if(linha_file!=NULL) fclose(veiculo_file);
		return FILE_ERROR;
	}

	LINHA_HEADER *linha_header = bin_get_header_linha(linha_file);
	VEICULO_HEADER *veiculo_header = bin_get_header_veiculo(veiculo_file);

	// verifica status dos arquivos
	if (linha_header->status == '0' || veiculo_header->status == '0') {
		free(linha_header); free(veiculo_header);
		fclose(veiculo_file); fclose(linha_file);
		return FILE_ERROR;
	}


	VEICULO *veiculo = NULL;
	LINHA *linha = NULL;

	char codLinhastr[7];
	int count=0;

	while ((veiculo=bin_get_veiculo(veiculo_file, NULL, NULL)) != NULL) {
		if (veiculo->removido=='0') {
			veiculo_delete(&veiculo);
			continue;
		}

		itoa(veiculo->codLinha, codLinhastr);
		// busca a linha com codLinha específico
		linha = bin_get_linha(linha_file, "codLinha", codLinhastr);

		// volta pro primeiro registro
		fseek(linha_file, 82, SEEK_SET);

		if (linha != NULL) {
			print_veiculo(veiculo);
			//printf("codLinha: %d\n",veiculo->codLinha);
			print_linha(linha);
			linha_delete(&linha);
			printf("\n");
			count++;
		}


		veiculo_delete(&veiculo);
	}

	free(linha_header);
	free(veiculo_header);

	fclose(veiculo_file);
	fclose(linha_file);

	if (count == 0)
		return REG_NULL;

	return 0;
}

ERROR operation16(char *veic_f, char *linha_f, char *index_f) {
	FILE *veiculo_file = fopen(veic_f, "rb"),
		 *linha_file = fopen(linha_f, "rb");

	// verifica se arquivos existem
	if (veiculo_file == NULL) {
		if(linha_file!=NULL) fclose(linha_file);
		return FILE_ERROR;
	}
	if (linha_file == NULL) {
		if(linha_file!=NULL) fclose(veiculo_file);
		return FILE_ERROR;
	}


	INDEX_HEADER *index_header = bin_get_header_index(index_f);
	if (index_header==NULL) {
		fclose(veiculo_file);
		fclose(linha_file);
		return FILE_ERROR;
	}
	///


	LINHA_HEADER *linha_header = bin_get_header_linha(linha_file);
	VEICULO_HEADER *veiculo_header = bin_get_header_veiculo(veiculo_file);

	// verifica status dos arquivos
	if (linha_header->status == '0' || veiculo_header->status == '0' || index_header->status=='0') {
		free(linha_header); free(veiculo_header); free(index_header);
		fclose(veiculo_file); fclose(linha_file);
		return FILE_ERROR;
	}


	VEICULO *veiculo = NULL;
	LINHA *linha = NULL;
	int64 byteoffset;
	int count = 0;

	while ((veiculo=bin_get_veiculo(veiculo_file, NULL, NULL)) != NULL) {

		if (veiculo->removido=='0') {
			veiculo_delete(&veiculo);
			continue;
		}

		// busca o byteoffset da linha com o codLinha específico na arvore B
		byteoffset = btree_search(index_f, index_header->noRaiz, veiculo->codLinha);

		// se o byteoffset não for -1, o registro existe, imprimindo os dados
		if (byteoffset != -1) {
			
			// lê a Linha na posição encontrada
			fseek(linha_file, byteoffset, SEEK_SET);
			linha = bin_get_linha(linha_file, NULL, NULL);
			
			print_veiculo(veiculo);
			print_linha(linha);
			printf("\n");
			
			linha_delete(&linha);
			count++;
		}

		veiculo_delete(&veiculo);
	}

	free(index_header);
	free(linha_header);
	free(veiculo_header);
	fclose(veiculo_file);
	fclose(linha_file);

	if (count == 0)
		return REG_NULL;

	return 0;
}





// funcão para imprimir toda a árvore (para Debug)
ERROR op22(char *bin_f) {
	INDEX_HEADER *index_header = bin_get_header_index(bin_f);
	btree_print(bin_f, 0);
	free(index_header);
}
