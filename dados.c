#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "csvparse.h"
#include "binparse.h"

/*
char *modelo;
	char *categoria;*/

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

LINHA_HEADER *header_linha_create(FILE *fp) {
	LINHA_HEADER *header = (LINHA_HEADER *)malloc(sizeof(LINHA_HEADER));

	header->status = '0';
	header->nroRegistros = 0;
	header->nroRegRemovidos = 0;
	header->byteProxReg = 82;

	header_linha_get_descr(fp, header);

	return header;
}

void print_linha(LINHA *linha) {
	printf("%d ", linha->codLinha);
	printf("%c ", linha->aceitaCartao);
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