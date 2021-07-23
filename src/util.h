#ifndef UTIL_H
#define UTIL_H

#define RRN(p, psize) (int)(p/psize)

#include "dados.h"

int convertePrefixo(const char* str);
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
char *readline(FILE *in);
void preenche_lixo(char *str, int i, int size);
void get_mes_nome(char *date, char *mes_nome);
void itoa(int num, char *buffer);
int v_linha_comp(const void *v1, const void *v2);
int linha_comp(const void *l1, const void *l2);

#endif