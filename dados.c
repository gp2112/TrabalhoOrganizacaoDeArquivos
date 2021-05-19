#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "csvparse.h"
#include "binparse.h"


// libera memória do veiculo/linha //
void linha_delete(LINHA **linha) {
	free((*linha)->nomeLinha);
	free((*linha)->corLinha);
	free(*linha);
	*linha = NULL;
}

void veiculo_delete(VEICULO **veiculo) {
	free((*veiculo)->modelo);
	free((*veiculo)->categoria);
	free(*veiculo);
	*veiculo = NULL;
}
///////////////////////////////////


// inicia um header de linha om valores iniciais
LINHA_HEADER *header_linha_create(FILE *fp) {
	LINHA_HEADER *header = (LINHA_HEADER *)malloc(sizeof(LINHA_HEADER));

	header->status = '0';
	header->nroRegistros = 0;
	header->nroRegRemovidos = 0;
	header->byteProxReg = 82;

	header_linha_get_descr(fp, header);

	return header;
}

// AAAA-MM-DD
void get_mes_nome(char *date, char *mes_nome) {
	// separa os parametros da data por 0 -> AAAA\0MM\0DD
	date[4]='\0'; date[7]='\0';

	int mes = atoi(date+5); 

	switch (mes) {
		case (1):
			strcpy(mes_nome, "janeiro");
			break;
		case (2):
			strcpy(mes_nome, "fevereiro");
			break;
		case (3):
			strcpy(mes_nome, "marco");
			break;
		case (4):
			strcpy(mes_nome, "abril");
			break;
		case (5):
			strcpy(mes_nome, "maio");
			break;
		case (6):
			strcpy(mes_nome, "junho");
			break;
		case (7):
			strcpy(mes_nome, "julho");
			break;
		case (8):
			strcpy(mes_nome, "agosto");
			break;
		case (9):
			strcpy(mes_nome, "setembro");
			break;
		case (10):
			strcpy(mes_nome, "outubro");
			break;
		case (11):
			strcpy(mes_nome, "novembro");
			break;
		case (12):
			strcpy(mes_nome, "dezembro");
			break;
	}
}

void print_linha(LINHA *linha) {
	
	if (strcmp(linha->nomeLinha, "NULO") == 0)
		printf("Nome da linha: campo com valor nulo\n");
	else printf("Nome da linha: %s\n", linha->nomeLinha);

	if (strcmp(linha->corLinha, "NULO") == 0)
		printf("Cor que descreve a linha: campo com valor nulo\n");
	else printf("Cor que descreve a linha: %s\n", linha->corLinha);

	printf("Aceita cartao: ");
	if (linha->aceitaCartao == 'S')
		printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
	else if (linha->aceitaCartao == 'N')
		printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
	else if (linha->aceitaCartao == 'F')
		printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
	else printf("campo com valor nulo\n");

	printf("\n");
}

void print_veiculo(VEICULO *veiculo) {
	if (strcmp(veiculo->prefixo, "NULO") == 0)
		printf("Prefixo do veiculo: campo com valor nulo\n");
	else printf("Prefixo do veiculo: %s\n", veiculo->prefixo);

	if (strcmp(veiculo->modelo, "NULO")==0)
		printf("Modelo do veiculo: campo com valor nulo\n");
	else printf("Modelo do veiculo: %s\n", veiculo->modelo);

	if (strcmp(veiculo->categoria, "NULO")==0)
		printf("Categoria do veiculo: campo com valor nulo\n");
	else printf("Categoria do veiculo: %s\n", veiculo->categoria);

	if (strcmp(veiculo->data, "NULO")==0)
		printf("Data de entrada do veiculo na frota: campo com valor nulo\n");
	else {
		char mes_nome[11]; get_mes_nome(veiculo->data, mes_nome);
		printf("Data de entrada do veiculo na frota: %s de %s de %s\n", veiculo->data+8, mes_nome, veiculo->data);
	}

	if (veiculo->quantidadeLugares == -1)
		printf("Quantidade de lugares sentados disponíveis: campo com valor nulo\n");
	else printf("Quantidade de lugares sentados disponíveis: %d\n", veiculo->quantidadeLugares);

	printf("\n");
}