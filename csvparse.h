#ifndef CSVPARSE_H
#define CSVPARSE_H

/*
LÊ ARQUIVO DE ENTRADA .CSV
STRUCTS DE CARRO E PEDESTRE

*/

#include "dados.h"


VEICULO_HEADER *get_veiculo_header(FILE *fp);

VEICULO *get_veiculo(FILE *fp); // Lê o próximo veículo do csv

LINHA_HEADER *get_linha_header(FILE *fp);

LINHA *get_linha(FILE *fp);

#endif