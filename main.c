#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvparse.h"
#include "binout.h"
#include "binparse.h"

#define ERROR char
#define FILE_ERROR 1 
#define REG_NULL 2

// Se a string tiver aspas, incrementa 1 no ponteiro 
// e substitui a última aspa por \0
// retorna o ponteiro para o fim da palavra
char *parse_str_aspas(char **str) {
	//printf("%s\n", *str);
	if ((*str)[0] != '"')
		return NULL;
	(*str)++;
	int i=0;
	while ((*str)[i] != '"') {
		if ((*str)[i] == '\0') // se encontrar \0 antes da aspa dupla, substitui por espaço
			(*str)[i] = ' ';
		i++;
	}
	(*str)[i] = '\0';
	return (*str)+i+2;
}

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
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

ERROR operation4(char *bin_fname) {
	FILE *bin_f=fopen(bin_fname, "rb");

	if (bin_f == NULL)
		return FILE_ERROR;

	LINHA *linha = NULL;

	while ((linha=bin_get_linha(bin_f, NULL, NULL)) != NULL) {
		if (linha->removido == '1')
			print_linha(linha);
		linha_delete(&linha);
	}

	fclose(bin_f);
	return 0;
}


ERROR operation6(char *bin_fname, char *param, char *value) {
	FILE *bin_f = fopen(bin_fname, "rb");

	if (bin_f == NULL)
		return FILE_ERROR;

	LINHA *linha = bin_get_linha(bin_f, param, value);
	if (linha == NULL) {
		fclose(bin_f);
		return REG_NULL;
	}

	do {
		print_linha(linha);
		linha_delete(&linha);

	} while ((linha=bin_get_linha(bin_f, param, value))!=NULL);


	fclose(bin_f);
	return 0;
}

ERROR operation8(char *bin_fname, int n) {

	FILE *bin_f = fopen(bin_fname, "ab");

	if (bin_f == NULL)
		return FILE_ERROR;

	char *line, *temp;

	LINHA *linha = NULL;

	LINHA_HEADER *header = bin_get_header_linha(bin_f);
	//printf("%lld\n", header->byteProxReg);
	header->status = '0';
	header_linha_alter_status(bin_f, '0');

	for (int i=0; i<n; i++) {

		linha = (LINHA *)malloc(sizeof(LINHA));

		line = readline(stdin);
		
		temp = strtok(line, " ");
		if (strcmp(temp, "NULO") == 0)
			linha->codLinha = -1;

		else linha->codLinha = atoi(temp);

		temp = strtok(NULL, " ");

		linha->aceitaCartao = temp[1];

		temp = strtok(NULL, " ");

		if (strcmp(temp, "NULO") == 0) {
			linha->nomeLinha = NULL;
			linha->tamanhoNome = 0;
		
		} else {
			char *aux = temp;
			temp = parse_str_aspas(&aux);
			linha->tamanhoNome = strlen(aux);
			linha->nomeLinha = (char*)calloc(linha->tamanhoNome+1, sizeof(char));
			strcpy(linha->nomeLinha, aux);
		}

		if (strcmp(temp, "NULO") == 0) {
			linha->corLinha = NULL;
			linha->tamanhoCor = 0;
		}
		else {
			parse_str_aspas(&temp);
			linha->tamanhoCor = strlen(temp);
			linha->corLinha = (char*)calloc(linha->tamanhoCor+1, sizeof(char));
			strcpy(linha->corLinha, temp);
		}
		linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;
		linha->removido = '1';
		print_linha(linha);
		
		escreve_linha(bin_f, header, linha);
		linha_delete(&linha);
	}

	header->status = '1';
	header_linha_alter_status(bin_f, '1');

	fclose(bin_f);

	binarioNaTela(bin_fname);

	return 0;
}


int main(int argc, char const *argv[]) {
	char *csv_fname, *bin_fname, *param, *value;
	char *line = readline(stdin), *tmp=NULL;
	int op;

	ERROR erro;

	tmp = strtok(line, " ");
	op = atoi(tmp);

	switch (op) {
		case (1):
			break;

		case (2):
			csv_fname = strtok(NULL, " ");
			bin_fname = strtok(NULL, " ");
			erro = operation2(csv_fname, bin_fname);
			break;

		case (3):
			
			break;

		case (4):
			bin_fname = strtok(NULL, " ");
			erro = operation4(bin_fname);
			break;

		case (5):
			break;

		case (6):
			bin_fname = strtok(NULL, " ");
			param = strtok(NULL, " ");
			value = strtok(NULL, " ");
			parse_str_aspas(&value);
			erro = operation6(bin_fname, param, value);
			break;

		case (7):
			break;

		case (8):
			bin_fname = strtok(NULL, " ");
			int n = atoi(strtok(NULL, " "));
			erro = operation8(bin_fname, n);
			break;
	}


	if (erro == FILE_ERROR)
		printf("Falha no processamento do arquivo.\n");
	else if (erro == REG_NULL)
		printf("Registro inexistente.\n");

	free(line);
	return 0;
}