#include <stdio.h>
#include <stdlib.h>
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