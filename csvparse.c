#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

#define BUFFER_SIZE 64

char *readline(FILE *in) {
	char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
	char c; int i=0;

	while ((c=fgetc(in)) == '\n' || c=='\r')
		if (c == EOF) return NULL;


	do {
		if (i>0 && i%BUFFER_SIZE == 0)
			buffer = (char *)realloc(buffer, BUFFER_SIZE * (int)(1 + BUFFER_SIZE/i)*sizeof(char));

		buffer[i++] = c;

	} while ((c=fgetc(in)) != '\n' && c!='\r' && c!=EOF);

	buffer[i] = '\0';
	return buffer;
}


LINHA *get_linha(FILE *fp) {
	LINHA *linha = (LISTA *)malloc(sizeof(LISTA));

	char *line = readline(fp), *temp=NULL;
	if (line == NULL) return NULL;

	temp = strtok(line, ",");
	linha->codLinha = atoi(temp);

	temp = strtok(NULL, ",");
	linha->aceitaCartao = temp[0];

	temp = strtok(NULL, ",");
	linha->nomeLinha = (char *)calloc(strlen(temp)+1, sizeof(char));
	linha->nomeLinha = strcpy(linha->nomeLinha, temp);

	temp = strtok(NULL, ",");
	linha->corLinha = (char *)calloc(strlen(temp)+1, sizeof(char));
	linha->corLinha = strcpy(linha->corLinha, temp);

	linha->removido = '0';
	linha->tamanhoNome = strlen(linha->nomeLinha);
	linha->tamanhoCor = strlen(linha->corLinha);

	linha->tamanhoRegistro = 18 + linha->tamanhoNome + linha->tamanhoCor;

	free(line);

	return linha;
}