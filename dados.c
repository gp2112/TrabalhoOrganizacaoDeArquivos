#include <stdio.h>
#include <stdlib.h>
#include "dados.h"
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