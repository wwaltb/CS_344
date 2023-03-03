#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    // check number of arguments
    if(argc != 2) {
        fprintf(stderr, "usage: keygen Keylength\n");
        return 1;
    }

    // check that keylength is an integer
    if(!atoi(argv[1])) {
        fprintf(stderr, "usage: keygen Keylength\n");
        return 1;
    }

    // initialize random number generator
    time_t t;
    srand((unsigned) time(&t));
    
    // generate random key and output to stdout
    for(int i = 0; i < atoi(argv[1]); i++) {
        int r = rand() % 27;
        char c = (r == 26) ? 32 : r + 65;
        printf("%c", c);
    }
    printf("\n");

    return 0;
}