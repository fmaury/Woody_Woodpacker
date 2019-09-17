#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

extern char	*rc4(char *input, size_t len, char *key);

static char             *keygen(void)
{
    char        *key;
    size_t      count = 0;
    char        c;

    int fd = open("/dev/random", O_RDONLY);
    key = malloc(17);
    while (count < 16)
    {
        if (read(fd, &c, 1) < 0)
            return (0);
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c<= 'z') || (c >= '0' && c <= '9'))
            key[count++] = c;
    }
    close(fd);
    return (key);
}

int main(int argc, char **argv)
{
	int fd = open("/dev/random", O_RDONLY);
	unsigned int len = atoi(argv[1]);
	char *input = malloc(len);
	char *output = malloc(len);
	char * key = keygen();
	read(fd, input, len);
	// for (int i = 0; i < len; i++) {
	// 	printf("%02hhX", input[i]);
	// }
	// printf("\n      ==> ");
	memcpy(output, input, len);
	rc4(output, len, key);

	// for (int i = 0; i < len; i++) {
	// 	printf("%02hhX", output[i]);
	// }
	rc4(output, len, key);
	// printf("\n");
	// for (int i = 0; i < len; i++) {
	// 	printf("%02hhX", output[i]);
	// }
	printf("%s %d\n", !memcmp(input, output, len) ? "TRUE" : "FALSE", len);

	return (0);
}
