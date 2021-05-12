#ifndef BINPARSE_H
#define BINPARSE_H

#include "dados.h"



VEICULO *bin_get_veiculo(FILE *fp, int codLinha);


LINHA *bin_get_linha(FILE *fp, int codLinha);

#ifndef