#ifndef UTIL_H
#define UTIL_H

int convertePrefixo(char* str);
void binarioNaTela(char *nomeArquivoBinario);
void scan_quote_string(char *str);
char *readline(FILE *in);
void preenche_lixo(char *str, int i, int size);
void get_mes_nome(char *date, char *mes_nome);

#endif