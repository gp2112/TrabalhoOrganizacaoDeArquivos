#ifndef BINOUT_H
#define BINOUT_H

/*
GERA ARQUIVO BINÁRIO DAS STRUCTS 
DE VEICULOS E LINHAS

*/

void escreve_header_veiculo(FILE *fp, HEADER_VEICULO *hv);
void escreve_header_linha(FILE *fp, HEADER_LINHA *hl);

#endif