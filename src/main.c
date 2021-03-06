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

		case (10):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			erro = operation10(bin_fname, index_name);
			break;

		case (11):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			scanf("%s", param);
			scan_quote_string(value);
			erro = operation11(bin_fname, index_name, value);
			break;

		case (12):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			scanf("%s", param);
			scanf("%d", &n);
			erro = operation12(bin_fname, index_name, n);
			break;

		case (13):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			scanf("%d", &n);
			erro = operation13(bin_fname, index_name, n);
			break;

		case (14):
			scanf("%s", bin_fname);
			scanf("%s", index_name);
			scanf("%d", &n);
			erro = operation14(bin_fname, index_name, n);
			break;

		case (15):
			scanf("%s", bin_fname);
			scanf("%s", csv_fname);
			scanf("%s", param);
			scanf("%s", param);
			erro = operation15(bin_fname, csv_fname);
			break;

		case (16):
			scanf("%s", bin_fname);
			scanf("%s", csv_fname);
			scanf("%s", param);
			scanf("%s", param);
			scanf("%s", index_name);
			erro = operation16(bin_fname, csv_fname, index_name);
			break;

		case (17):
			scanf("%s", bin_fname);
			scanf("%s", param);
			scanf("%s", value);
			erro = operation17(bin_fname, param);
			if (erro==0) binarioNaTela(param);
			break;

		case (18):
			scanf("%s", bin_fname);
			scanf("%s", param);
			scanf("%s", value);
			erro = operation18(bin_fname, param);
			if (erro==0) binarioNaTela(param);
			break;

		case (19):
			scanf("%s", bin_fname);
			scanf("%s", param);
			scanf("%s", value);
			scanf("%s", value);
			erro = operation19(bin_fname, param);
			break;

		case (-1):
			scanf("%s", bin_fname);
			erro = op22(bin_fname);
			break;
	}


	if (erro == FILE_ERROR)
		printf("Falha no processamento do arquivo.\n");
	else if (erro == REG_NULL)
		printf("Registro inexistente.\n");

	return 0;
}