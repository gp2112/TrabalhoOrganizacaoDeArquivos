#include <stdio.h>
#include <stdlib.h>
#include "dados.h"


LINHA *get_linha(FILE *fp) {
	LINHA *linha = (LISTA *)malloc(sizeof(LISTA));

	char *temp;
	fscanf("%[,^\n]", temp);

	return linha;
}