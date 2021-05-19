#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"


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
	linha->nomeLinha = (char *)calloc(linha->tamanhoNome+1, sizeof(char));

	fread(linha->nomeLinha, sizeof(char), linha->tamanhoNome, fp);

	fread(&linha->tamanhoCor, sizeof(int), 1, fp);
	linha->corLinha = (char *)calloc(linha->tamanhoCor+1, sizeof(char));

	fread(linha->corLinha, sizeof(char), linha->tamanhoCor, fp);

	return linha;
}

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
		if (linha->removido != '0' && strcmp(linha->nomeLinha, nomeLinha)==0)
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
		
		if (linha->removido != '0' && strcmp(linha->corLinha, cor)==0)
			break;
		linha_delete(&linha);

	}

	return linha;
}

LINHA *bin_get_linha(FILE *fp, char *campo, char *value) {
	
	if (strcmp(campo, "codLinha") == 0)
		return bin_get_linha_by_cod(fp, atoi(value));

	if (strcmp(campo, "aceitaCartao")==0)
		return bin_get_linha_by_cartao(fp, value[0]);

	if (strcmp(campo, "nomeLinha")==0)
		return bin_get_linha_by_nome(fp, value);

	if (strcmp(campo, "corLinha")==0)
		return bin_get_linha_by_cor(fp, value);

	return NULL;

}

