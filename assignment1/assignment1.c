#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./students student_info1.txt\n");
        return EXIT_FAILURE;
    }

    struct movie *list = processMovies(argv[1]);
}