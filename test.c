#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void print_bin(unsigned char byte)
{
    int i = CHAR_BIT; /* however many bits are in a byte on your platform */
    while(i--) {
        putchar('0' + ((byte >> i) & 1)); /* loop through and print the bits */
    }
}


int main(int argc, char **argv){
    char c1 = (char) 0;
    char c2 = '\0';
    print_bin(c1);
    printf("\n");
    print_bin(c2);
    printf("\n");
    c1 += 0b101;
    print_bin(c1);
    printf("\n");
}