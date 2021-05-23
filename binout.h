#ifndef BINOUT_H
#define BINOUT_H

/*
GERA ARQUIVO BINÁRIO DAS STRUCTS 
DE CARRO E PEDESTRE
*/

void escreve_header_veiculo(FILE *fp, VEICULO_HEADER *hv);
void escreve_header_linha(FILE *fp, LINHA_HEADER *hl);

// Indentificar campos nulos como "NULO" //
void escreve_veiculo(FILE *fp, VEICULO_HEADER *h, VEICULO *v);

void escreve_linha(FILE *fp, LINHA_HEADER *header, LINHA *linha);
//////////////////////////////////////////

void header_linha_alter_status(FILE *fp, char status);
void header_linha_alter_byteproxreg(FILE *fp, int64 byteProxReg);
void header_linha_alter_regs(FILE *fp, int regs);
void header_linha_alter_reg_removidos(FILE *fp, int nroRegRemovidos);


void header_veiculo_alter_status(FILE *fp, char status);
void header_veiculo_alter_byteproxreg(FILE *fp, int64 byteProxReg);
void header_veiculo_alter_regs(FILE *fp, int regs);
void header_veiculo_alter_reg_removidos(FILE *fp, int nroRegRemovidos);

#endif