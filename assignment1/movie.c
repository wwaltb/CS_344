#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct movie {
    char *title;
    int year;
    char **languages;
    float rating;
    struct movie *next;
};

struct movie *createMovie(char *line) {
    struct movie *movie = malloc(sizeof(struct movie));

    // tokenize line using strtok_r:
    char *token;
    char *saveptr;

    // first token is title
    token = strtok_r(line, ",", &saveptr);
    movie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(movie->title, token);

    // second token is year
    token = strtok_r(line, ",", &saveptr);
    movie->year = atoi(token);

    // third token is languages


    // last token is rating


    movie->next = NULL;
    return movie;
}

/**/
struct movie *processMovies(char *filename) {
    // open filename for reading
    FILE *moviesFile = fopen(filename, "r");

    // create the head of linked list:
    struct movie *head = NULL;
    struct movie *curr = NULL;

    // read the file line by line:
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    bool headers = true;

    while((nread = getline(&line, &len, moviesFile)) != -1) {
        if(headers == true) {
            printf("Headers line.\n");
            headers = false;
            continue;
        }
        printf(line);
        //struct movie *newMovie = createMovie(line);

    }
}