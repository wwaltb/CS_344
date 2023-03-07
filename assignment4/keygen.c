#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "cipher.h"

int main(int argc, char **argv) {
    // check number of arguments
    if(argc != 2) {
        fprintf(stderr, "USAGE: %s Keylength\n", argv[0]);
        return 1;
    }

    // check that keylength is an integer
    if(!atoi(argv[1])) {
        fprintf(stderr, "USAGE: %s Keylength\n", argv[0]);
        return 1;
    }

    // initialize random number generator
    time_t t;
    srand((unsigned) time(&t));
    
    // generate random key and output to stdout
    int i;
    for(i = 0; i < atoi(argv[1]); i++) {
        char c = toChar(rand() % 27);
        printf("%c", c);
    }
    printf("\n");

    return 0;
}