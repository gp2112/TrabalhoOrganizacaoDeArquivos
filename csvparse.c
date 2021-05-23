#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"

#define BUFFER_SIZE 64

// preenche lixo de string com '@', a patir da posição i do '\0' até o tamanho total
void preenche_lixo(char *str, int i, int size) {
	if (i>=size) return ;
	str[i++] = '\0';
	for (; i<size; i++)
		str[i] = '@';
}

// funcao para ler um arquivo ate uma quebra de linha e armazenar em um buffer e retornar tudo log em seguida
char *readline(FILE *in) { 
	char *buffer = NULL;
	char c; int i = 0;

	while ((c = fgetc(in)) == '\n' || c == '\r'); // le eventuais linhas vazias antes de ler o conteudo
	if (c == EOF) return NULL;
		

	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	do {
		if (i>0 && (i+1)%BUFFER_SIZE == 0)
			buffer = (char *)realloc(buffer, BUFFER_SIZE * (int)(1 + (i+1)/BUFFER_SIZE)*sizeof(char));

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
		linha->removido = '0';
		temp++; // desloca ponteiro para o próximo byte
	}
	else linha->removido = '1';
	linha->codLinha = atoi(temp);
	// verifica se está removido


	temp = strtok(NULL, ",");
	linha->aceitaCartao = temp[0];

	temp = strtok(NULL, ",");
	if (strcmp(temp, "NULO")==0) {
		linha->tamanhoNome = 0;
		linha->nomeLinha = NULL;
	} else {
		linha->tamanhoNome = strlen(temp);
		linha->nomeLinha = (char *)calloc(linha->tamanhoNome+1, sizeof(char));
		linha->nomeLinha = strcpy(linha->nomeLinha, temp);
	}

	temp = strtok(NULL, ",");
	if (strcmp(temp, "NULO")==0) {
		linha->tamanhoCor = 0;
		linha->corLinha = NULL;
	} else {
		linha->tamanhoCor = strlen(temp);
		linha->corLinha = (char *)calloc(linha->tamanhoCor+1, sizeof(char));
		linha->corLinha = strcpy(linha->corLinha, temp);
	}
	
	linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;

	free(line);

	return linha;
}

// aloca o header do csv na descricao
// ???????? LEAK ?????????????????? ///////
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
		printf("%s\n", linha);
		free(linha);
	}
	linha = readline(fp); temp=NULL;

	if (linha == NULL) return NULL;
	if (strlen(linha) == 0) {
		free(linha);
		return NULL;
	}
	VEICULO *vehicle = (VEICULO*)malloc(sizeof(VEICULO));

	temp = strtok(linha,",");

	if (temp[0] == '*') {
		vehicle->removido = '0';
		temp++; // desloca ponteiro para o próximo byte
	}
	else
		vehicle->removido = '1';
	
	
	strcpy(vehicle->prefixo, temp);
	preenche_lixo(vehicle->prefixo, strlen(temp)+1, 6);
	
	temp = strtok(NULL,",");
	if (strcmp(temp, "NULO")==0)
		preenche_lixo(vehicle->data, 0, 11);
	else {
		strcpy(vehicle->data,temp);
		preenche_lixo(vehicle->data, strlen(temp), 11);
	}

	temp = strtok(NULL,","); 
	vehicle->quantidadeLugares = atoi(temp);

	temp = strtok(NULL,","); 
	vehicle->codLinha = atoi(temp);


	temp = strtok(NULL,",");
	if (strcmp(temp, "NULO")==0) {
		vehicle->tamanhoModelo = 0;
		vehicle->modelo = NULL;
	} else {
		vehicle->modelo = (char*) calloc(strlen(temp)+1,sizeof(char));
		vehicle->modelo = strcpy(vehicle->modelo,temp);
		vehicle->tamanhoModelo = strlen(temp);
	}

	temp = strtok(NULL,",");
	
	if (strcmp(temp, "NULO")==0) {
		vehicle->categoria = NULL;
		vehicle->tamanhoCategoria = 0;
	} else {
		vehicle->categoria = (char*) calloc(strlen(temp)+1,sizeof(char));
		vehicle->categoria = strcpy(vehicle->categoria,temp);
		vehicle->tamanhoCategoria = strlen(temp);
	}

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

