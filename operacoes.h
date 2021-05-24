#ifndef OPERACOES_H

#define OPERACOES_H

#include "csvparse.h"
#include "binout.h"
#include "binparse.h"

#define ERROR char
#define FILE_ERROR 1 
#define REG_NULL 2

ERROR operation1(char *csv_fname, char *bin_fname);
ERROR operation2(char *csv_fname, char *bin_fname);
ERROR operation3(char *bin_fname);
ERROR operation4(char *bin_fname);
ERROR operation5(char *bin_fname, char *param, char *value);
ERROR operation6(char *bin_fname, char *param, char *value);
ERROR operation7(char *bin_fname, int n);
ERROR operation8(char *bin_fname, int n);


#endif