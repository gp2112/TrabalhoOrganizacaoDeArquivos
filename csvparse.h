#ifndef CSVPARSE_H
#define CSVPARSE_H

/*
LÊ ARQUIVO DE ENTRADA .CSV
STRUCTS DE CARRO E PEDESTRE

*/

#include "dados.h"

// Lê o próximo veículo/linha do csv

VEICULO *get_veiculo(FILE *fp); 

LINHA *get_linha(FILE *fp);

void header_linha_get_descr(FILE *fp, LINHA_HEADER *header);

void header_veiculo_get_descr(FILE *fp, VEICULO_HEADER *header);

#endif