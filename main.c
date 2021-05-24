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
	char csv_fname[20], bin_fname[20];
	
	int op; // codigo da operacao 
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
			scanf("%s", bin_fname);
			char param[15]; scanf("%s", param);
			char value[100];
			scan_quote_string(value);
			erro = operation5(bin_fname, param, value);
			break;

		case (6):
			scanf("%s", bin_fname);
			char param[15]; scanf("%s", param);
			char value[100];
			scan_quote_string(value);
			erro = operation6(bin_fname, param, value);
			break;

		case (7):
			scanf("%s", bin_fname);
			int n; scanf("%d", &n);
			erro = operation7(bin_fname, n);
			break;

		case (8):
			scanf("%s", bin_fname);
			int n; scanf("%d", &n);
			erro = operation8(bin_fname, n);
			break;
	}


	if (erro == FILE_ERROR)
		printf("Falha no processamento do arquivo.\n");
	else if (erro == REG_NULL)
		printf("Registro inexistente.\n");

	return 0;
}