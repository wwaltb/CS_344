#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
    char *title;
    char *year;
    char **languages;
    char *rating;
    struct movie *next;
};

char **getLanguages(char *langToken) {
    //char **langArray = malloc(5 * sizeof(char*));
    char **langArray = NULL;

    return langArray;
}

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
    movie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(movie->year, token);
    //movie->year = atoi(token);

    // third token is languages
    token = strtok_r(line, ",", &saveptr);
    movie->languages = getLanguages(token);

    // last token is rating
    token = strtok_r(line, ",", &saveptr);
    movie->rating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(movie->rating, token);
    //movie->rating = strtod(token, NULL);

    movie->next = NULL;
    return movie;
}

/**/
struct movie *processMovies(char *filename) {
    // open filename for reading
    FILE *moviesFile = fopen(filename, "r");

    // create the head of linked list
    struct movie *head = NULL;
    struct movie *curr = NULL;

    // read the file line by line:
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int headers = 1;

    while((nread = getline(&line, &len, moviesFile)) != -1) {
        // skip the first line containing the headers
        if(headers == 1) {
            headers = 0;
            continue;
        }

        // create new movie node based on line data
        struct movie *newMovie = createMovie(line);

        if(head == NULL) {
            head = newMovie;
            curr = newMovie;
        }
        else {
            curr->next = newMovie;
            curr = newMovie;
        }
    }

    return head;
}

void printMovies(struct movie *head) {
    struct movie *curr = head;
    while(curr != NULL) {
        printf("%s %s %s\n", curr->title, curr->year, curr->rating);
        curr = curr->next;
    }
}