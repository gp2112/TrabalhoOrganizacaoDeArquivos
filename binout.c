#include <stdio.h>
#include <stdlib.h>
#include "dados.h"

// FALTA TRATAR O LIXO!!!!!!

// escreve todo o header 
void escreve_header_linha(FILE *fp, LINHA_HEADER *header) {
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);
	fwrite(&header->status, sizeof(char), 1, fp);
	fwrite(&header->byteProxReg, sizeof(int64), 1, fp);
	fwrite(&header->nroRegistros, sizeof(int), 1, fp);
	fwrite(&header->nroRegRemovidos, sizeof(int), 1, fp);

	// strings não incluem \0
	fwrite(header->descreveCodigo, sizeof(char), 15, fp);
	fwrite(header->descreveCartao, sizeof(char), 13, fp);
	fwrite(header->descreveNome, sizeof(char), 13, fp);
	fwrite(header->descreveCor, sizeof(char), 24, fp);

}

// altera campo status do header
void header_linha_alter_status(FILE *fp, char status) {
	fseek(fp, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, fp);
}

// altera campo byteProxReg do header
void header_linha_alter_byteproxreg(FILE *fp, int64 byteProxReg) {
	fseek(fp, 1, SEEK_SET);
	fwrite(&byteProxReg, sizeof(int64), 1, fp);
}

// altera campo registros do header
void header_linha_alter_regs(FILE *fp, int regs) {
	fseek(fp, 9, SEEK_SET);
	fwrite(&regs, sizeof(int), 1, fp);
}

// altera campo regremovidos do header
void header_linha_alter_reg_removidos(FILE *fp, int nroRegRemovidos) {
	fseek(fp, 13, SEEK_SET);
	fwrite(&nroRegRemovidos, sizeof(int), 1, fp);
}

void escreve_linha(FILE *fp, LINHA_HEADER *header, LINHA *linha) {

	if (ftell(fp) != header->byteProxReg) 
		fseek(fp, header->byteProxReg, SEEK_SET);

	fwrite(&linha->removido, sizeof(char), 1, fp);
	fwrite(&linha->tamanhoRegistro, sizeof(int), 1, fp);
	fwrite(&linha->codLinha, sizeof(int), 1, fp);
	fwrite(&linha->aceitaCartao, sizeof(char), 1, fp);
	fwrite(&linha->tamanhoNome, sizeof(int), 1, fp);
	fwrite(linha->nomeLinha, sizeof(char), linha->tamanhoNome+1, fp);
	fwrite(&linha->tamanhoCor, sizeof(int), 1, fp);
	fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor+1, fp);

	header->byteProxReg = ftell(fp);
	if (linha->removido == '1') {
		header->nroRegistros++;
		header_linha_alter_regs(fp, header->nroRegistros);
	}
	else {
		header->nroRegRemovidos++;
		header_linha_alter_reg_removidos(fp, header->nroRegRemovidos);
	}

	
	header_linha_alter_byteproxreg(fp, header->byteProxReg);

}
