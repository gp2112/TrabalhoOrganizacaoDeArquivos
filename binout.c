#include <stdio.h>
#include <stdlib.h>
#include "dados.h"

/// Operações de escrita no arquivo binário ///

void escreve_header_index(FILE *fp, INDEX_HEADER *header) {
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);
	fwrite(&header->status, sizeof(char), 1, fp);
	fwrite(&header->noRaiz, sizeof(int), 1, fp);
	fwrite(&header->RRNproxNo, sizeof(int), 1, fp);
	char lixo[68];
	for (int i=0; i<68; i++) lixo[i]='@';

	fwrite(lixo, sizeof(char), 68, fp);

}

void escreve_index_data(FILE *fp, INDEX_REG *data) {
	// vai pra segunda página caso esteja na primeira (header)
	fseek(fp, 77*data->RRNdoNo, SEEK_SET);

	fwrite(&data->folha, sizeof(char), 1, fp);
	fwrite(&data->nroChavesIndexadas, sizeof(int), 1, fp);
	fwrite(&data->RRNdoNo, sizeof(int), 1, fp);
	for (int i=0; i<ORDEM-1; i++) {
		fwrite(&data->children[i], sizeof(int), 1, fp);
		fwrite(&data->keys[i], sizeof(int), 1, fp);
		fwrite(&data->pos[i], sizeof(int64), 1, fp);
	}
	fwrite(&data->children[ORDEM-1], sizeof(int), 1, fp);

}

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

	// se não tiver na posição do próximo registro
	// vai para a posicao header->byteProxReg
	if (ftell(fp) != header->byteProxReg) 
		fseek(fp, header->byteProxReg, SEEK_SET);
	
	fwrite(&linha->removido, sizeof(char), 1, fp);
	fwrite(&linha->tamanhoRegistro, sizeof(int), 1, fp);
	fwrite(&linha->codLinha, sizeof(int), 1, fp);
	fwrite(&linha->aceitaCartao, sizeof(char), 1, fp);
	fwrite(&linha->tamanhoNome, sizeof(int), 1, fp);
	fwrite(linha->nomeLinha, sizeof(char), linha->tamanhoNome, fp);
	fwrite(&linha->tamanhoCor, sizeof(int), 1, fp);
	fwrite(linha->corLinha, sizeof(char), linha->tamanhoCor, fp);

	// atribui a posição do próximo registro à posição atual
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

// escreve todo o header 
void escreve_header_veiculo(FILE *fp, VEICULO_HEADER *header) {
	if (ftell(fp) > 0) fseek(fp, 0, SEEK_SET);
	
	fwrite(&header->status, sizeof(char), 1, fp);
	fwrite(&header->byteProxReg, sizeof(int64), 1, fp);
	fwrite(&header->nroRegistros, sizeof(int), 1, fp);
	fwrite(&header->nroRegRemovidos, sizeof(int), 1, fp);

	// strings não incluem \0
	fwrite(header->descrevePrefixo, sizeof(char), 18, fp);
	fwrite(header->descreveData, sizeof(char), 35, fp);
	fwrite(header->descreveLugares, sizeof(char), 42, fp);
	fwrite(header->descreveLinha, sizeof(char), 26, fp);
	fwrite(header->descreveModelo, sizeof(char), 17, fp);
	fwrite(header->descreveCategoria, sizeof(char), 20, fp);

}

// altera campo status do header
void header_veiculo_alter_status(FILE *fp, char status) {
	fseek(fp, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, fp);
}

// altera campo byteProxReg do header
void header_veiculo_alter_byteproxreg(FILE *fp, int64 byteProxReg) {
	fseek(fp, 1, SEEK_SET);
	fwrite(&byteProxReg, sizeof(int64), 1, fp);
}

// altera campo registros do header
void header_veiculo_alter_regs(FILE *fp, int regs) {
	fseek(fp, 9, SEEK_SET);
	fwrite(&regs, sizeof(int), 1, fp);
}

// altera campo regremovidos do header
void header_veiculo_alter_reg_removidos(FILE *fp, int nroRegRemovidos) {
	fseek(fp, 13, SEEK_SET);
	fwrite(&nroRegRemovidos, sizeof(int), 1, fp);
}

void escreve_veiculo(FILE *fp, VEICULO_HEADER *header, VEICULO *veiculo) {

	// se não tiver na posição do próximo registro
	// vai para a posicao header->byteProxReg
	if (ftell(fp) != header->byteProxReg) 
		fseek(fp, header->byteProxReg, SEEK_SET);

	fwrite(&veiculo->removido, sizeof(char), 1, fp);
	fwrite(&veiculo->tamanhoRegistro, sizeof(int), 1, fp);
	fwrite(veiculo->prefixo, sizeof(char), 5, fp);
	fwrite(veiculo->data, sizeof(char), 10, fp);
	fwrite(&veiculo->quantidadeLugares, sizeof(int), 1, fp);
	fwrite(&veiculo->codLinha, sizeof(int), 1, fp);
	fwrite(&veiculo->tamanhoModelo, sizeof(int), 1, fp);
	fwrite(veiculo->modelo, sizeof(char), veiculo->tamanhoModelo, fp);
	fwrite(&veiculo->tamanhoCategoria, sizeof(int), 1, fp);
	fwrite(veiculo->categoria, sizeof(char), veiculo->tamanhoCategoria, fp);

	// atribui a posição do próximo registro à posição atual
	header->byteProxReg = ftell(fp);

	if (veiculo->removido == '1') {
		header->nroRegistros++;
		header_veiculo_alter_regs(fp, header->nroRegistros);
	}
	else {
		header->nroRegRemovidos++;
		header_veiculo_alter_reg_removidos(fp, header->nroRegRemovidos);
	}

	header_veiculo_alter_byteproxreg(fp, header->byteProxReg);

}
