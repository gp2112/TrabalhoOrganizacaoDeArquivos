#ifndef BINPARSE_H
#define BINPARSE_H

#include "dados.h"


VEICULO_HEADER *bin_get_header_veiculo(FILE *fp);

VEICULO *bin_get_veiculo(FILE *fp, char *campo, char *value);

LINHA_HEADER *bin_get_header_linha(FILE *fp);

LINHA *bin_get_linha(FILE *fp, char *campo, char *value);


#endif