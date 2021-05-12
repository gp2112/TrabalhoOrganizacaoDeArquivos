#include <stdio.h>
#include <stdlib.h>
#include "dados.h"

LINHA_HEADER *get_linha_header(FILE *fp) {
	LINHA_HEADER *header = (LINHA_HEADER *)malloc(sizeof(LINHA_HEADER));

	return header;
}

LINHA *get_linha(FILE *fp) {
	LINHA *linha = (LISTA *)malloc(sizeof(LISTA));



	return linha;
}