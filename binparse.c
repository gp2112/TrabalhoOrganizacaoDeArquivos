#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

/// Operações de leitura de arquivo binário ///

INDEX_HEADER *bin_get_header_index(FILE *fp) {
	INDEX_HEADER *header = (INDEX_HEADER *)malloc(sizeof(INDEX_HEADER));
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);

	fread(&header->status, sizeof(char), 1, fp);
	fread(&header->noRaiz, sizeof(int), 1, fp);
	fread(&header->RRNproxNo, sizeof(int), 1, fp);

	return header;
}


INDEX_REG *bin_get_index_reg(FILE *fp) {
	INDEX_REG *reg = (INDEX_REG *)malloc(sizeof(INDEX_REG));
	if (ftell(fp) < 77) fseek(fp, 77, SEEK_SET);

	fread(&reg->folha, sizeof(char), 1, fp);
	fread(&reg->nroChavesIndexadas, sizeof(int), 1, fp);
	fread(&reg->RRNdoNo, sizeof(int), 1, fp);
	for (int i=0; i<ORDEM; i++) {
		fread(&reg->ps[i], sizeof(int), 1, fp);
		fread(&reg->cs[i], sizeof(int), 1, fp);
		fread(&reg->prs[i], sizeof(int64), 1, fp);
	}
	fread(&reg->ps[ORDEM-1], sizeof(int), 1, fp);

	return reg;
}


// lê e retorna o header do arquivo binário de linha
LINHA_HEADER *bin_get_header_linha(FILE *fp) {
	LINHA_HEADER *header = (LINHA_HEADER *)malloc(sizeof(LINHA_HEADER));

	// se o ponteiro não estiver no início do arquivo, aponta para o 0
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);

	fread(&header->status, sizeof(char), 1, fp);
	fread(&header->byteProxReg, sizeof(int64), 1, fp);
	fread(&header->nroRegistros, sizeof(int), 1, fp);
	fread(&header->nroRegRemovidos, sizeof(int), 1, fp);
	fread(header->descreveCodigo, sizeof(char), 15, fp);
	fread(header->descreveCartao, sizeof(char), 13, fp);
	fread(header->descreveNome, sizeof(char), 13, fp);
	fread(header->descreveCor, sizeof(char), 24, fp);

	return header;
}

// lê a linha estando na posição correta
LINHA *bin_read_linha(FILE *fp) {
	// se o ponteiro estiver no cabeçalho, fssek para a primeira linha
	if (ftell(fp) < 82) fseek(fp, 82, SEEK_SET);

	LINHA *linha = (LINHA *)malloc(sizeof(LINHA));

	if (fread(&linha->removido, sizeof(char), 1, fp) != 1) {
		free(linha);
		return NULL;
	}
	fread(&linha->tamanhoRegistro, sizeof(int), 1, fp);
	fread(&linha->codLinha, sizeof(int), 1, fp);

	fread(&linha->aceitaCartao, sizeof(char), 1, fp);

	fread(&linha->tamanhoNome, sizeof(int), 1, fp);
	if (linha->tamanhoNome == 0)
		linha->nomeLinha = NULL;
	else {
		linha->nomeLinha = (char *)calloc(linha->tamanhoNome+1, sizeof(char));
		fread(linha->nomeLinha, sizeof(char), linha->tamanhoNome, fp);
	}

	fread(&linha->tamanhoCor, sizeof(int), 1, fp);
	if (linha->tamanhoCor == 0)
		linha->corLinha = NULL;
	else {
		linha->corLinha = (char *)calloc(linha->tamanhoCor+1, sizeof(char));
		fread(linha->corLinha, sizeof(char), linha->tamanhoCor, fp);
	}

	return linha;
}

/* bin_get_linha_by_X:
 *    Busca uma linha no arquivo
 *	  pelo parâmetro X
*/
LINHA *bin_get_linha_by_cod(FILE *fp, int cod) {

	LINHA *linha = NULL;

	while (1) {
		linha = bin_read_linha(fp);
		if (linha == NULL) return NULL;


		if (linha->removido != '0' && linha->codLinha==cod)
			break;
		linha_delete(&linha);
	}

	return linha;
}

LINHA *bin_get_linha_by_cartao(FILE *fp, char aceitaCartao) {

	LINHA *linha = NULL;

	while (1) {
		linha = bin_read_linha(fp);
		if (linha == NULL) return NULL;
		if (linha->removido != '0' && linha->aceitaCartao == aceitaCartao)
			break;
		linha_delete(&linha);

	}

	return linha;

}

LINHA *bin_get_linha_by_nome(FILE *fp, char *nomeLinha) {
	
	LINHA *linha = NULL;

	while (1) {
		linha = bin_read_linha(fp);
		if (linha == NULL) return NULL;

		if (linha->removido != '0' && linha->nomeLinha!=NULL && strcmp(linha->nomeLinha, nomeLinha)==0)
			break;
		linha_delete(&linha);

	}

	return linha;
}

LINHA *bin_get_linha_by_cor(FILE *fp, char *cor) {
	LINHA *linha = NULL;

	while (1) {
		linha = bin_read_linha(fp);
		if (linha == NULL) return NULL;
		
		if (linha->removido != '0' && linha->corLinha!=NULL && strcmp(linha->corLinha, cor)==0)
			break;
		linha_delete(&linha);

	}

	return linha;
}

// Dado um campo, retonar a primeira linha encontrada com o valor correspondente
LINHA *bin_get_linha(FILE *fp, char *campo, char *value) {
	if (campo == NULL || value == NULL)
		return bin_read_linha(fp); 

	if (strcmp(campo, "codLinha") == 0)
		return bin_get_linha_by_cod(fp, atoi(value));

	if (strcmp(campo, "aceitaCartao")==0)
		return bin_get_linha_by_cartao(fp, value[0]);

	if (strcmp(campo, "nomeLinha")==0)
		return bin_get_linha_by_nome(fp, value);

	if (strcmp(campo, "corLinha")==0)
		return bin_get_linha_by_cor(fp, value);

	// se nao for nenhum campo valido, retorna a proxima linha

}

VEICULO_HEADER *bin_get_header_veiculo(FILE *fp){
	VEICULO_HEADER *header = (VEICULO_HEADER *)malloc(sizeof(VEICULO_HEADER));
	fread(&header->status, sizeof(char), 1, fp);
	fread(&header->byteProxReg, sizeof(int64), 1, fp);
	fread(&header->nroRegistros, sizeof(int), 1, fp);
	fread(&header->nroRegRemovidos, sizeof(int), 1, fp);
	fread(header->descrevePrefixo, sizeof(char), 18, fp);
	fread(header->descreveData, sizeof(char), 35, fp);
	fread(header->descreveLugares, sizeof(char), 42, fp);
	fread(header->descreveLinha, sizeof(char), 26, fp);
	fread(header->descreveModelo, sizeof(char), 17, fp);
	fread(header->descreveCategoria, sizeof(char), 20, fp);

	return header;
}


// lê o veiculo estando na posição correta
VEICULO *bin_read_veiculo(FILE *fp) {
	// se o ponteiro estiver no cabeçalho, fssek para a primeira linha
	if (ftell(fp) < 175) fseek(fp, 175, SEEK_SET);

	VEICULO *veiculo = (VEICULO *)malloc(sizeof(VEICULO));

	if (fread(&veiculo->removido, sizeof(char), 1, fp) != 1) {
		free(veiculo);
		return NULL;
	}
	fread(&veiculo->tamanhoRegistro, sizeof(int), 1, fp);
	fread(veiculo->prefixo, sizeof(char), 5, fp); veiculo->prefixo[5]='\0';
	fread(veiculo->data, sizeof(char), 10, fp); veiculo->data[10]='\0';
	fread(&veiculo->quantidadeLugares, sizeof(int), 1, fp);
	fread(&veiculo->codLinha, sizeof(int), 1, fp);
	
	fread(&veiculo->tamanhoModelo, sizeof(int), 1, fp);
	if (veiculo->tamanhoModelo == 0)
		veiculo->modelo = NULL;
	else {
		veiculo->modelo = (char*)calloc(veiculo->tamanhoModelo+1, sizeof(char));
		fread(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, fp);
	}
	fread(&veiculo->tamanhoCategoria, sizeof(int), 1, fp);
	if (veiculo->tamanhoCategoria == 0)
		veiculo->categoria = NULL;
	else {
		veiculo->categoria = (char*)calloc(veiculo->tamanhoCategoria+1, sizeof(char));
		fread(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, fp);
	}

	return veiculo;
}

/* bin_get_veiculo_by_X:
 *    Busca um veiculo no arquivo
 *	  pelo parâmetro X
*/

VEICULO *bin_get_veiculo_by_prefixo(FILE *fp, char* prefixo) {

	VEICULO *veiculo = NULL;

	while (1) {
		veiculo = bin_read_veiculo(fp);
		if (veiculo == NULL){
			return NULL;
		}

		if (veiculo->removido != '0' && !strcmp(veiculo->prefixo,prefixo)){
			break;
		}
		veiculo_delete(&veiculo);
	}

	return veiculo;
}

VEICULO *bin_get_veiculo_by_data(FILE *fp, char* data) {

	VEICULO *veiculo = NULL;

	while (1) {
		veiculo = bin_read_veiculo(fp);
		if (veiculo == NULL){
			return NULL;
		}

		if (veiculo->removido != '0' && !strcmp(veiculo->data,data)){
			break;
		}
		veiculo_delete(&veiculo);
	}

	return veiculo;
}

VEICULO *bin_get_veiculo_by_quantidadeLugares(FILE *fp, int quantidadeLugares) {

	VEICULO *veiculo = NULL;

	while (1) {
		veiculo = bin_read_veiculo(fp);
		if (veiculo == NULL){
			return NULL;
		}

		if (veiculo->removido != '0' && veiculo->quantidadeLugares == quantidadeLugares){
			break;
		}
		veiculo_delete(&veiculo);
	}

	return veiculo;
}

VEICULO *bin_get_veiculo_by_modelo(FILE *fp, char* modelo) {

	VEICULO *veiculo = NULL;

	while (1) {
		veiculo = bin_read_veiculo(fp);
		if (veiculo == NULL){
			return NULL;
		}

		if (veiculo->removido != '0' && veiculo->modelo != NULL 
									&&!strcmp(veiculo->modelo,modelo)){
			break;
		}
		veiculo_delete(&veiculo);
	}

	return veiculo;
}

VEICULO *bin_get_veiculo_by_categoria(FILE *fp, char* categoria) {

	VEICULO *veiculo = NULL;

	while (1) {
		veiculo = bin_read_veiculo(fp);
		if (veiculo == NULL){
			return NULL;
		}

		if (veiculo->removido != '0' && veiculo->modelo!=NULL
								 && !strcmp(veiculo->categoria,categoria)){
			break;
		}
		veiculo_delete(&veiculo);
	}

	return veiculo;
}

// Dado um campo, retonar a primeira linha encontrada com o valor correspondente
VEICULO *bin_get_veiculo(FILE *fp, char *campo, char *value) {
	
	// se nao for nenhum campo valido, retorna a proxima linha
	if (campo == NULL || value == NULL)
		return bin_read_veiculo(fp); 

	if (strcmp(campo, "prefixo") == 0)
		return bin_get_veiculo_by_prefixo(fp, value);

	if (strcmp(campo, "data")==0)
		return bin_get_veiculo_by_data(fp, value);

	if (strcmp(campo, "quantidadeLugares")==0)
		return bin_get_veiculo_by_quantidadeLugares(fp, atoi(value));

	if (strcmp(campo, "modelo")==0)
		return bin_get_veiculo_by_modelo(fp, value);

	if (strcmp(campo, "categoria")==0)
		return bin_get_veiculo_by_categoria(fp, value);


}