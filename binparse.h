#ifndef BINPARSE_H
#define BINPARSE_H

#include "dados.h"

INDEX_HEADER *bin_get_header_index(FILE *fp);

INDEX_REG *bin_get_index_reg(FILE *fp);

VEICULO_HEADER *bin_get_header_veiculo(FILE *fp);

VEICULO *bin_get_veiculo(FILE *fp, char *campo, char *value);

INDEX_HEADER *bin_get_index(FILE *fp);

LINHA_HEADER *bin_get_header_linha(FILE *fp);

LINHA *bin_get_linha(FILE *fp, char *campo, char *value);



#endif