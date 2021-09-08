#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
#define AWORD               uint16_t
#define IWORD               int16_t

    int minus1  = -1;
    printf("%i\n", minus1);

    AWORD addr = (-1);
    printf("%i\n", addr);

    IWORD newaddr = addr;
    printf("%i\n", newaddr);

    addr += 2;
    printf("%i\n", addr);

    return 0;
}