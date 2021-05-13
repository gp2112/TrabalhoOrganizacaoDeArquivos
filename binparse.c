#include <stdio.h>
#include <stdlib.h>
#include "dados.h"

/*
typedef struct {

	int codLinha;
	char aceitaCartao;
	char *nomeLinha;
	char *corLinha;
	char removido;
	int tamanhoRegistro;
	int tamanhoNome;
	int tamanhoCor;

} LINHA;

*/

LINHA *bin_get_linha(FILE *fp, int cod) {
	char removido;
	int tamanhoRegistro, codLinha;

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