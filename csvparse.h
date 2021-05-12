#ifndef CSVPARSE_H
#define CSVPARSE_H

/*
LÊ ARQUIVO DE ENTRADA .CSV
STRUCTS DE CARRO E PEDESTRE

*/

#include "dados.h"


VEICULO *get_veiculo(FILE *fp); // Lê o próximo veículo do csv

LINHA *get_linha(FILE *fp);

#endif