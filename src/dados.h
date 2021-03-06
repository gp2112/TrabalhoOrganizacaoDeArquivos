#ifndef DADOS_H
#define DADOS_H

#ifndef int64
#define int64 long long int
#endif

#ifndef ORDEM
#define ORDEM 5
#endif

typedef struct {
	char folha;
	int nroChavesIndexadas;
	int RRNdoNo;
	int children[ORDEM];
	int keys[ORDEM-1]; // ordenado !
	int64 pos[ORDEM-1];

} INDEX_REG;

typedef struct {
	char status;
	int noRaiz;
	int RRNproxNo;

} INDEX_HEADER;


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

	char prefixo[6]; 
	char data[11]; // AAAA-MM-DD
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

INDEX_HEADER *header_index_create();

LINHA_HEADER *header_linha_create(FILE *fp);

VEICULO_HEADER *header_veiculo_create(FILE *fp);

void print_linha(LINHA *linha);

void print_veiculo(VEICULO *veiculo);

VEICULO *veiculo_input();
LINHA *linha_input();

#endif