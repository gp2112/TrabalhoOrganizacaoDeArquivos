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
}

// coleta veiculo com os parametros do usuario
VEICULO *veiculo_input() {

		VEICULO *veiculo = (VEICULO *)malloc(sizeof(VEICULO));

		char temp[100];
		
		scan_quote_string(veiculo->prefixo);
		if (strcmp(veiculo->prefixo, "NULO")==0)
			preenche_lixo(veiculo->prefixo, 0, 5);
		else
			preenche_lixo(veiculo->prefixo, strlen(veiculo->prefixo), 5);
		

		scan_quote_string(veiculo->data);
		if (strcmp(veiculo->data, "NULO")==0)
			preenche_lixo(veiculo->data, 0, 11);
		else
			preenche_lixo(veiculo->data, strlen(veiculo->data), 11);

		scanf("%d %d", &veiculo->quantidadeLugares, &veiculo->codLinha);

		scan_quote_string(temp);
		if (strcmp(temp, "NULO")==0) {
			veiculo->tamanhoModelo = 0;
			veiculo->modelo = NULL;
		}
		else {
			veiculo->tamanhoModelo = strlen(temp);
			veiculo->modelo = (char *)calloc(veiculo->tamanhoModelo+1, sizeof(char));
			strcpy(veiculo->modelo, temp);
		}

		scan_quote_string(temp);
		if (strcmp(temp, "NULO")==0) {
			veiculo->tamanhoCategoria = 0;
			veiculo->categoria = NULL;
		}
		else {
			veiculo->tamanhoCategoria = strlen(temp);
			veiculo->categoria = (char *)calloc(veiculo->tamanhoCategoria+1, sizeof(char));
			strcpy(veiculo->categoria, temp);
		}

		veiculo->tamanhoRegistro = 31 + veiculo->tamanhoCategoria + veiculo->tamanhoModelo;
		veiculo->removido = '1';
}

LINHA *linha_input() {
		LINHA *linha = (LINHA *)malloc(sizeof(LINHA));
		
		char temp[100];

		scanf("%d", &linha->codLinha);

		scan_quote_string(&linha->aceitaCartao);

		scan_quote_string(temp);
		if (strcmp(temp, "NULO") == 0) {
			linha->nomeLinha = NULL;
			linha->tamanhoNome = 0;
		
		} else {
			linha->tamanhoNome = strlen(temp);
			linha->nomeLinha = (char*)calloc(linha->tamanhoNome+1, sizeof(char));
			strcpy(linha->nomeLinha, temp);
		}

		scan_quote_string(temp);
		if (strcmp(temp, "NULO") == 0) {
			linha->corLinha = NULL;
			linha->tamanhoCor = 0;
		}
		else {
			linha->tamanhoCor = strlen(temp);
			linha->corLinha = (char*)calloc(linha->tamanhoCor+1, sizeof(char));
			strcpy(linha->corLinha, temp);
		}
		linha->tamanhoRegistro = 13 + linha->tamanhoNome + linha->tamanhoCor;
		linha->removido = '1';
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

