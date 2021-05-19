#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvparse.h"
#include "binout.h"
#include "binparse.h"


int main(int argc, char const *argv[]) {

	/*FILE *f_csv = fopen(argv[1], "r"),
		 *f_bin = fopen(argv[2], "wb");



	LINHA *linha = NULL;

	LINHA_HEADER *header_linha = header_linha_create(f_csv);

	escreve_header_linha(f_bin, header_linha);
	
	int i = 0;
	while ((linha=get_linha(f_csv)) != NULL) {
		printf("%d - ", i++);
		printf("%d ", linha->codLinha);
		printf("%c ", linha->aceitaCartao);
		printf("%s ", linha->nomeLinha);
		printf("%s ", linha->corLinha);
		printf("%c ", linha->removido);
		printf("%d ", linha->tamanhoRegistro);
		printf("%d ", linha->tamanhoNome);
		printf("%d\n", linha->tamanhoCor);

		escreve_linha(f_bin, header_linha, linha);

		linha_delete(&linha);
	}

	free(header_linha);
	header_linha_alter_status(f_bin, '1');
	fclose(f_bin);
	fclose(f_csv);*/
	FILE *f = fopen(argv[1], "rb");
	LINHA *linha = NULL;

	while (1) {
		linha = bin_get_linha(f, argv[2], argv[3]);
		
		if (linha == NULL) break;
		print_linha(linha);

		linha_delete(&linha);
	}

	fclose(f);

	return 0;
}
/*

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

*/