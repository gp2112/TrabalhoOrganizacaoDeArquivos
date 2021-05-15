#include <stdio.h>
#include <stdlib.h>
#include "dados.h"


HEADER_LINHA *bin_get_header_linha(FILA *fp) {
	HEADER_LINHA *header = (HEADER_LINHA *)malloc(sizeof(HEADER_LINHA));

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

LINHA *bin_get_linha(FILE *fp, int cod) {
	char removido;
	int tamanhoRegistro, codLinha;

	// se o ponteiro estiver no cabeçalho, fssek para a primeira linha
	if (ftell(fp) < 82) fseek(fp, 82, SEEK_SET);

	do {
		// lê e verifica se chegou no fim do arquivo
		if (fread(&removido, sizeof(char), 1, fp) != 1) return NULL;
		if (fread(&tamanhoRegistro, sizeof(int), 1, fp) != 1) return NULL;
		if (fread(&codLinha, sizeof(int), 1, fp) != 1) return NULL;

	} while (codLinha != cod && removido == '0');

	LINHA *linha = (LINHA *)malloc(sizeof(LINHA));

	linha->removido = removido;
	linha->tamanhoRegistro = tamanhoRegistro;
	linha->codLinha = codLinha;

	fread(&linha->aceitaCartao, sizeof(char), 1, fp);

	fread(&linha->tamanhoNome, sizeof(int), 1, fp);
	linha->nomeLinha = (char *)calloc(linha->tamanhoNome, sizeof(char));

	fread(linha->nomeLinha, sizeof(char), linha->tamanhoNome+1, fp);

	fread(&linha->tamanhoCor, sizeof(int), 1, fp);
	linha->corLinha = (char *)calloc(linha->tamanhoCor+1, sizeof(char));

	fread(linha->corLinha, sizeof(char), linha->tamanhoCor, fp);

	return linha;
}