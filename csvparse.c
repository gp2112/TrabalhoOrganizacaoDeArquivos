#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

#define BUFFER_SIZE 64

// funcao para ler um arquivo ate uma quebra de linha e armazenar em um buffer e retornar tudo log em seguida
char *readline(FILE *in) { 
	char *buffer = NULL;
	char c; int i = 0;

	while ((c = fgetc(in)) == '\n' || c == '\r'); // le eventuais linhas vazias antes de ler o conteudo
	if (c == EOF) return NULL;
		

	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	do {
		if (i>0 && i%BUFFER_SIZE == 0)
			buffer = (char *)realloc(buffer, BUFFER_SIZE * (int)(1 + BUFFER_SIZE/i)*sizeof(char));

		buffer[i++] = c;

	} while ((c = fgetc(in)) != '\n' && c != '\r' && c != EOF); 

	buffer[i] = '\0';

	return buffer;
}


LINHA *get_linha(FILE *fp) {
	char *line, *temp;

	// se tiver no ínicio do arquivo, ignora o header do csv
	if (ftell(fp) == 0) {
		line = readline(fp);
		free(line);
	}

	line = readline(fp), temp=NULL;

	if (line == NULL) return NULL;
	if (strlen(line) == 0) {
		free(line);
		return NULL;
	}
	//printf("%d\n", EOF);
	LINHA *linha = (LINHA *)malloc(sizeof(LINHA));

	temp = strtok(line, ",");
	if (temp[0] == '*') {
		linha->removido = '1';
		temp++; // desloca ponteiro para o próximo byte
	}
	else linha->removido = '0';
	linha->codLinha = atoi(temp);
	// verifica se está removido


	temp = strtok(NULL, ",");
	linha->aceitaCartao = temp[0];

	temp = strtok(NULL, ",");
	linha->nomeLinha = (char *)calloc(strlen(temp)+1, sizeof(char));
	linha->nomeLinha = strcpy(linha->nomeLinha, temp);


	temp = strtok(NULL, ",");
	linha->corLinha = (char *)calloc(strlen(temp)+1, sizeof(char));
	linha->corLinha = strcpy(linha->corLinha, temp);

	
	linha->tamanhoNome = strlen(linha->nomeLinha);
	linha->tamanhoCor = strlen(linha->corLinha);

	linha->tamanhoRegistro = 18 + linha->tamanhoNome + linha->tamanhoCor;

	free(line);

	return linha;
}

// aloca o header do csv na descricao
void header_linha_get_descr(FILE *fp, LINHA_HEADER *header) {
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);
	char *line = readline(fp), *temp=NULL;

	temp = strtok(line, ",");
	strcpy(header->descreveCodigo, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveCartao, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveNome, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveCor, temp);


	free(line);
}

VEICULO *get_veiculo(FILE *fp){
	char *linha, *temp;

	// se tiver no ínicio do arquivo, ignora o header do csv
	if (ftell(fp) == 0) {
		linha = readline(fp);
		free(linha);
	}
	linha = readline(fp), temp=NULL;

	if (linha == NULL) return NULL;
	if (strlen(linha) == 0) {
		free(linha);
		return NULL;
	}
	VEICULO *vehicle = (VEICULO*)malloc(sizeof(VEICULO));

	temp = strtok(linha,',');
	if (temp[0] == '*') {
		vehicle->removido = '1';
		temp++; // desloca ponteiro para o próximo byte
	}
	else {
		vehicle->removido = '0';
		}
	
	strcpy(vehicle->prefixo,temp);
	if (strlen(temp) != 5)
	{
		int num = strlen (temp);
		while (num < 5)
		{
			vehicle->prefixo [num] ='@';
			num++;

		}
	}
	
	temp = strtok(NULL,',');
	strcpy(vehicle->data,temp);
	if (strlen(temp) != 10)
	{
		int num = strlen (temp);
		while (num < 10)
		{
			vehicle->data [num] ='@';
			num++;

		}
	}
	temp = strtok(NULL,','); 
	vehicle->quantidadeLugares = atoi(temp);
	temp = strtok(NULL,','); 
	vehicle->codLinha = atoi(temp);
	temp = strtok(NULL,','); 
	vehicle->tamanhoModelo = atoi(temp);
	temp = strtok(NULL,','); 
	vehicle->modelo = (char*) calloc(strlen(temp)+1,sizeof(char));
	vehicle->modelo = strcpy(vehicle->modelo,temp);
	temp = strtok(NULL,','); 
	vehicle->tamanhoCategoria = atoi(temp);
	vehicle->categoria = (char*) calloc(strlen(temp)+1,sizeof(char));
	vehicle->categoria = strcpy(vehicle->categoria,temp);
	vehicle->tamanhoRegistro = 31 + vehicle->tamanhoCategoria +vehicle->tamanhoModelo;	

	free(linha);
	return vehicle;
}

// aloca o header do csv na descricao
void header_veiculo_get_descr(FILE *fp, VEICULO_HEADER * header) {
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);
	char *line = readline(fp), *temp=NULL;

	temp = strtok(line, ",");
	strcpy(header->descrevePrefixo, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveData, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveLugares, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveLinha, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveModelo, temp);
	temp = strtok(NULL, ",");
	strcpy(header->descreveCategoria, temp);

	free(line);
}

