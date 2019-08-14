#include <unistd.h>

int main(int ac, char ** argv)
{
    write(1, "coucou\n", 7);
    return (0);
}