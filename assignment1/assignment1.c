#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

/*
 * getInput - prompts the user with a message and returns the input string
 * @message: message to prompt user with
 * @len: maximum length of the input string
 *
 * Return: string with user input
 */
char* getInput(char* message, int len) {
    char* input = calloc(len + 1, sizeof(char));
    printf("%s", message);
    scanf("%s", input);
    input[strcspn(input, "\n")] = 0;        // remove trailing \n
    return input;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_sampe_1.txt\n");
        return EXIT_FAILURE;
    }

    struct movie *list = processMovies(argv[1]);
    if(list == NULL) {
        return EXIT_FAILURE;
    }
    
    // main loop:
    int quit = 0;
    while (!quit) {
        // prompt user to enter choice from 1 to 4:
        printf("\n1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");

        int choice = atoi(getInput("Enter a choice from 1 to 4: ", 9));

        // call corresponding function:
        if(choice == 1) {
            int year = atoi(getInput("Enter the year for which you want to see movies: ", 9));
            printMoviesInYear(list, year);
        } else if(choice == 2) {
            printHighestRatedInEachYear(list);
        } else if(choice == 3) {
            char *language = getInput("Enter the language for which you want to see movies: ", 20);
            printMoviesInLanguage(list, language);
        } else if(choice == 4) {
            quit = 1;
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }
    
    // free the list of movies
    freeMovieList(list);
}