#ifndef DADOS_H
#define DADOS_H

#ifndef int64
#define int64 long long int
#endif

typedef struct {

	char status;
	int64 byteProxReg;
	int nroRegistros;
	int nroRegRemovidos;
	char descrevePrefixo[18];
	char descreveData[35];
	char descreveLugares[42];
	char descreveLinha[26];
	char descreveModelo[17];
	char descreveCategoria[20];

} VEICULO_HEADER;

typedef struct {

	char prefixo[5];
	char data[10]; // AAAA-MM-DD
	int quantidadeLugares;
	int codLinha;
	char *modelo;
	char *categoria;
	char removido;
	int tamanhoRegistro;
	int tamanhoModelo;
	int tamanhoCategoria;

} VEICULO;

typedef struct {

	char status;
	int64 byteProxReg;
	int nroRegistros;
	int nroRegRemovidos;
	char descreveCodigo[15];
	char descreveCartao[13];
	char descreveNome[13];
	char descreveCor[24];

} LINHA_HEADER;

typedef struct {

	int codLinha;
	char aceitaCartao;
	char *nomeLinha;
	char *corLinha;
	char removido;
	int tamanhoRegistro;
	int tamanhoNome;
	int tamanhoCor;

} LINHA;

void linha_delete(LINHA **linha);

void veiculo_delete(VEICULO **veiculo);

LINHA_HEADER *header_linha_create(FILE *fp);

VEICULO_HEADER *header_veiculo_create(FILE *fp);

#endif