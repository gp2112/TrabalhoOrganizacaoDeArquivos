#ifndef CSVPARSE_H
#define CSVPARSE_H

/*
LÊ ARQUIVO DE ENTRADA .CSV
STRUCTS DE CARRO E PEDESTRE

*/

#include "dados.h"


VEICULO *get_veiculo_csv(FILE *fp); // Lê o próximo veículo do csv

LINHA *get_linha_csv(FILE *fp);

#endif