#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
	char temp1[10], temp2[10];
	scanf("[,^\n]", temp1);
	scanf("[,^\n]", temp2);
	printf("%s\n", temp1);
	printf("%s\n", temp2);
	return 0;
}