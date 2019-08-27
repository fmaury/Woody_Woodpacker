#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void	swap(unsigned char *a, unsigned char *b)
{
	unsigned char t = *a;

	*a = *b;
	*b = t;
}

char	*rc4(char *input, size_t len, char *key)
{
	unsigned char S[256];
	int i = 0;
	int j = 0;
	int temp = 0;

	for (int i=0; i < 256; i++) {
		S[i] = i;
	}
	j = 0;
	for (int i=0; i < 256; i++) {
		j = (j + S[i] + key[i % strlen(key)]) % 256;
		swap(S + i, S + j);
	}
	i = 0;
	j = 0;
	for (int k = 0; k < strlen(input); k++)
	{
		i = (i+1) % 256;
		j = (j + S[i]) % 256;
		swap(S + i, S + j);
		temp = (S[i] + S[j]) % 256;
		input[k] ^= S[temp];
	}
	return input;
}

int main(int argc, char **argv)
{
	char *input = strdup(argv[1]);
	puts(input);
	rc4(input, strlen(argv[1]), argv[2]);

	for (int i = 0; i < strlen(input); i++) {
		printf("%02hhX-", input[i]);
	}
	return (0);
}