/*
 	Guilherme Ramos Costa Paixao - 11796079
 	Dennis Lemke Green - 112219108
 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes.h"
#include "util.h"



int main(int argc, char const *argv[]) {
	char csv_fname[20], bin_fname[20], index_name[20],
			param[15], value[100];


	int op, n; // codigo da operacao 
	scanf("%d", &op);


	ERROR erro;

	switch (op) {
		case (1):
			scanf("%s", csv_fname);
			scanf("%s^[\n\r]", bin_fname);
			erro = operation1(csv_fname, bin_fname);
			break;

		case (2):
			scanf("%s", csv_fname);
			scanf("%s^[\n\r]", bin_fname);
			erro = operation2(csv_fname, bin_fname);
			break;

		case (3):
			scanf("%s^[\n\r]", bin_fname);
			erro = operation3(bin_fname);
			break;

		case (4):
			scanf("%s^[\n\r]", bin_fname);
			erro = operation4(bin_fname);
			break;

		case (5):
			scanf("%s %s", bin_fname, param);
			scan_quote_string(value);
			erro = operation5(bin_fname, param, value);
			break;

		case (6):
			scanf("%s %s", bin_fname, param);
			scan_quote_string(value);
			erro = operation6(bin_fname, param, value);
			break;

		case (7):
			scanf("%s", bin_fname);
			scanf("%d", &n);
			erro = operation7(bin_fname, n);
			break;

		case (8):
			scanf("%s", bin_fname);
			scanf("%d", &n);
			erro = operation8(bin_fname, n);
			break;

		case (9):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			erro = operation9(bin_fname, index_name);
			break;

		case (11):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			scanf("%s", param);
			scan_quote_string(value);
			erro = operation11(bin_fname, index_name, value);
			break;
	}


	if (erro == FILE_ERROR)
		printf("Falha no processamento do arquivo.\n");
	else if (erro == REG_NULL)
		printf("Registro inexistente.\n");

	return 0;
}