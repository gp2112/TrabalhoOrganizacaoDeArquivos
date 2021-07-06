#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "csvparse.h"
#include "binparse.h"
#include "util.h"

/// Funções de manipulação de LINHA e VEICULO ///

// libera memória da linha //
void linha_delete(LINHA **linha) {
	if ((*linha)->nomeLinha != NULL)
		free((*linha)->nomeLinha);
	if ((*linha)->corLinha != NULL)
		free((*linha)->corLinha);
	free(*linha);
	*linha = NULL;
}

// libera memória do veiculo//
void veiculo_delete(VEICULO **veiculo) {
	if ((*veiculo)->modelo != NULL)
		free((*veiculo)->modelo);
	if ((*veiculo)->categoria)
		free((*veiculo)->categoria);
	free(*veiculo);
	*veiculo = NULL;
}
///////////////////////////////////


INDEX_HEADER *header_index_create() {
	INDEX_HEADER *index = (INDEX_HEADER *)malloc(sizeof(INDEX_HEADER));

	if (index != NULL) {
		index->status = '0';
		index->noRaiz = -1;
		index->RRNproxNo = 77;
	}

	return index;
}

// inicia um header de linha om valores iniciais
LINHA_HEADER *header_linha_create(FILE *fp) {
	LINHA_HEADER *header = (LINHA_HEADER *)malloc(sizeof(LINHA_HEADER));

	header->status = '0';
	header->nroRegistros = 0;
	header->nroRegRemovidos = 0;
	header->byteProxReg = 82;

	header_linha_get_descr(fp, header);

	return header;
}

VEICULO_HEADER *header_veiculo_create(FILE *fp) {
	VEICULO_HEADER *header = (VEICULO_HEADER *)malloc(sizeof(VEICULO_HEADER));

	header->status = '0';
	header->nroRegistros = 0;
	header->nroRegRemovidos = 0;
	header->byteProxReg = 175;

	header_veiculo_get_descr(fp, header);

	return header;
}

void print_linha(LINHA *linha) {
	
	if (linha->codLinha == -1)
		printf("Codigo da linha: campo com valor nulo\n");
	else printf("Codigo da linha: %d\n", linha->codLinha);

	if (linha->nomeLinha == NULL)
		printf("Nome da linha: campo com valor nulo\n");
	else printf("Nome da linha: %s\n", linha->nomeLinha);

	if (linha->corLinha == NULL)
		printf("Cor que descreve a linha: campo com valor nulo\n");
	else printf("Cor que descreve a linha: %s\n", linha->corLinha);

	printf("Aceita cartao: ");
	if (linha->aceitaCartao == 'S')
		printf("PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
	else if (linha->aceitaCartao == 'N')
		printf("PAGAMENTO EM CARTAO E DINHEIRO\n");
	else if (linha->aceitaCartao == 'F')
		printf("PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n");
	else printf("campo com valor nulo\n");

	printf("\n");
}

void print_veiculo(VEICULO *veiculo) {

	if (strlen(veiculo->prefixo)==0)
		printf("Prefixo do veiculo: campo com valor nulo\n");
	else printf("Prefixo do veiculo: %s\n", veiculo->prefixo);

	if (veiculo->modelo == NULL)
		printf("Modelo do veiculo: campo com valor nulo\n");
	else printf("Modelo do veiculo: %s\n", veiculo->modelo);

	if (veiculo->categoria == NULL)
		printf("Categoria do veiculo: campo com valor nulo\n");
	else printf("Categoria do veiculo: %s\n", veiculo->categoria);

	if (strlen(veiculo->data)==0)
		printf("Data de entrada do veiculo na frota: campo com valor nulo\n");
	else {
		char mes_nome[11]; get_mes_nome(veiculo->data, mes_nome);
		printf("Data de entrada do veiculo na frota: %s de %s de %s\n", veiculo->data+8, mes_nome, veiculo->data);
		veiculo->data[4]='-'; veiculo->data[7]='-';
	}

	if (veiculo->quantidadeLugares == -1)
		printf("Quantidade de lugares sentados disponiveis: campo com valor nulo\n");
	else printf("Quantidade de lugares sentados disponiveis: %d\n", veiculo->quantidadeLugares);

	printf("\n");
}

/*

typedef struct {
	char folha;
	int nroChavesIndexadas;
	int RRNdoNo;
	int ps[ORDEM];
	int cs[ORDEM-1]; // ordenado !
	int64 prs[ORDEM-1];

} INDEX_REG;

*/

INDEX_REG *create_indexreg() {
	if (btree == NULL) return NULL;

	INDEX_REG *index_reg = (INDEX_REG *)malloc(sizeof(INDEX_REG));
	if (index_reg == NULL)
		return NULL;


	for (int i=0; i<ORDEM-1; i++) {
		index_reg->cs[i] = -1;
		index_reg->ps[i] = -1;
		index_reg->prs[i] = -1;
	}
	index_reg->ps[i] = btree->children[i]==NULL ? -1 : btree->children[i]->rrn;


	return index_reg;
}