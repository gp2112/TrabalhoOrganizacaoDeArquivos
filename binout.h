#ifndef BINOUT_H
#define BINOUT_H

/*
GERA ARQUIVO BINÁRIO DAS STRUCTS 
DE CARRO E PEDESTRE
*/

void escreve_header_veiculo(FILE *fp, HEADER_VEICULO *hv);
void escreve_header_linha(FILE *fp, HEADER_LINHA *hl);

// Indentificar campos nulos como "NULO" //
void insere_veiculo(FILE *fp, VEICULO *v);

void insere_linha(FILE *fp, LINHA *v);
//////////////////////////////////////////


#endif