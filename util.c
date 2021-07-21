
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 64


/// Funções gerais utilizadas ///

int convertePrefixo(char* str) {

	/* O registro que tem essa string como chave foi removido */
	if(str[0] == '*')
		return -1;

	/* Começamos com o primeiro digito na ordem de 36^0 = 1 */
	int power = 1;

	/* Faz a conversão char por char para chegar ao resultado */
	int result = 0;
	for(int i = 0; i < 5; i++) {

		/* 
			Interpreta o char atual como se fosse um digito
			em base 36. Os digitos da base 36 são:
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
			E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
			S, T, U, V, W, X, Y, Z
		*/
		int cur_digit;
		/* Checa pelos digitos normais e os converte para números */
		if(str[i] >= '0' && str[i] <= '9')
			cur_digit = str[i] - '0';
		/* Checa pelas letras e as converte para números */
		else if(str[i] >= 'A' && str[i] <= 'Z')
			cur_digit = 10 + str[i] - 'A';

		/*
			Multiplica o digito atual pelo ordem da posição atual
			e adiciona no resultado
			Primeira posição:   36^0 = 1
			Segunda posição:    36^1 = 36
			Terceira posição:   36^2 = 1.296
			Quarta posição:     36^3 = 46.656
			Quinta posição:     36^4 = 1.679.616
		*/
		result += cur_digit * power;

		/* Aumenta a ordem atual */
		power *= 36;

	}

	return result;
}

void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


/* ---------------- EXTRA ----------------

OPCIONAL: dicas sobre scanf() e fscanf():

scanf("%[^,]", string) -> lê até encontrar o caractere ',', não incluindo o mesmo na leitura.

Exemplo de entrada: "Oi, esse é um exemplo."
Nesse caso, o scanf("%[^,]") tem como resultado a string "Oi";

scanf("%[^\"]", string) -> lê até encontrar o caractere '"', não incluindo o mesmo na leitura.
scanf("%[^\n]", string) -> lê até encontrar o fim da linha, não incluindo o '\n' na leitura.

scanf("%*c") --> lê um char e não guarda em nenhuma variável, como se tivesse ignorado ele

*/

// funcao para ler um arquivo ate uma quebra de linha e armazenar em um buffer e retornar tudo log em seguida
char *readline(FILE *in) { 
	char *buffer = NULL;
	char c; int i = 0;

	while ((c = fgetc(in)) == '\n' || c == '\r'); // le eventuais linhas vazias antes de ler o conteudo
	if (c == EOF) return NULL;
		

	buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	do {
		if (i>0 && (i+1)%BUFFER_SIZE == 0)
			buffer = (char *)realloc(buffer, BUFFER_SIZE * (int)(1 + (i+1)/BUFFER_SIZE)*sizeof(char));

		buffer[i++] = c;

	} while ((c = fgetc(in)) != '\n' && c != '\r' && c != EOF); 

	buffer[i] = '\0';

	return buffer;
}


// preenche lixo de string com '@', a patir da posição i do '\0' até o tamanho total
void preenche_lixo(char *str, int i, int size) {
	if (i>=size) return ;
	str[i++] = '\0';
	for (; i<size; i++)
		str[i] = '@';
}


// AAAA-MM-DD
void get_mes_nome(char *date, char *mes_nome) {
	// separa os parametros da data por 0 -> AAAA\0MM\0DD
	date[4]='\0'; date[7]='\0';

	int mes = atoi(date+5); 

	switch (mes) {
		case (1):
			strcpy(mes_nome, "janeiro");
			break;
		case (2):
			strcpy(mes_nome, "fevereiro");
			break;
		case (3):
			strcpy(mes_nome, "março");
			break;
		case (4):
			strcpy(mes_nome, "abril");
			break;
		case (5):
			strcpy(mes_nome, "maio");
			break;
		case (6):
			strcpy(mes_nome, "junho");
			break;
		case (7):
			strcpy(mes_nome, "julho");
			break;
		case (8):
			strcpy(mes_nome, "agosto");
			break;
		case (9):
			strcpy(mes_nome, "setembro");
			break;
		case (10):
			strcpy(mes_nome, "outubro");
			break;
		case (11):
			strcpy(mes_nome, "novembro");
			break;
		case (12):
			strcpy(mes_nome, "dezembro");
			break;
	}
}

void reverse(char *buffer, int size) {
	char aux;
	for (int i=0; i<size/2; i++) {
		aux = buffer[size-i-1];
		buffer[size-i-1] = buffer[i];
		buffer[i] = aux;
	}
}

// converte um int para sua representacao em string
void itoa(int num, char *buffer) {
	int digit, i=0;
	do {
		digit = num%10;
		buffer[i++] = '0'+digit;
	} while (((int)(num/=10))>0);
	buffer[i] = '\0';
	reverse(buffer, i);
}