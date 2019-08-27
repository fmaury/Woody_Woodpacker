#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  
    char buf[] = "\xeb\x1d\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\x48\x31\xc0\x40\xb7\x01\x5e\xb2\x07\xb0\x01\x0f\x05\xb8\x3c\x00\x00\x00\x0f\x05\xe8\xde\xff\xff\xff\x63\x6f\x75\x63\x6f\x75\x0a";
    printf("Size: %d bytes.\n", strlen(buf));
    void (*shellcode)() = (void((*)())) (buf);

    shellcode();

    return EXIT_SUCCESS;
}

