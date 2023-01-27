#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
    char *title;
    int year;
    char **languages;       // array of 5 languages
    double rating;
    struct movie *next;
};

/*
 * getLanguages - splits a string of languages into an array
 * @langToken: string containing language datar
 *
 * Return: array of languages with capacity 5
 */
char **getLanguages(char *langToken) {

    // initialize array of languages:
    char **langArray = malloc(5 * sizeof(char*));
    for(int i = 0; i < 5; i++) {
        langArray[i] = calloc(21, sizeof(char));
    }

    // tokenize langToken into languages:
    char *token;
    char *saveptr;

    // get rid of leading [
    token = strtok_r(langToken, "[", &saveptr);
    token = strtok_r(token, "]", &saveptr);
    
    // tokenize languages into array by ";"
    int i = 0;
    while((token = strtok_r(token, ";", &saveptr)) != NULL) {
        strcpy(langArray[i], token);

        token = NULL;
        i++;
    }

    return langArray;
}

/*
 * createMovie - creates a movie struct from a string of movie data
 * @line: string of movie data
 *
 * Return: movie struct with data from string
 */
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
    token = strtok_r(NULL, ",", &saveptr);
    movie->year = atoi(token);

    // third token is languages
    token = strtok_r(NULL, ",", &saveptr);
    movie->languages = getLanguages(token);

    // last token is rating
    token = strtok_r(NULL, ",", &saveptr);
    movie->rating = strtod(token, NULL);

    movie->next = NULL;
    return movie;
}

/*
 * processMovies - processes a movie data file into a linked list of movies
 * @filename: full name of the movie file
 *
 * Return: the head of the linked list of movies
 */
struct movie *processMovies(char *filename) {
    // open filename for reading
    FILE *moviesFile = fopen(filename, "r");
    if(moviesFile == NULL) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    // create the head of linked list
    struct movie *head = NULL;
    struct movie *curr = NULL;

    // read the file line by line:
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int headers = 1;
    int processed = 0;

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

        processed++;
    }

    printf("Processed file %s and parsed data for %d movies\n", filename, processed);

    return head;
}

/*
 * freeMovies - frees a list of movies
 * @head: the head of the linked list of movies
 */
void freeMovies(struct movie *head) {
    if(head == NULL) return;
    freeMovies(head->next);
    free(head);
}

/*
 * printMovies - prints all of the movie data of a list
 * @head: the head of the linked list of movies
 */
void printMovies(struct movie *head) {
    struct movie *curr = head;
    while(curr != NULL) {
        printf("%s %d %.1f", curr->title, curr->year, curr->rating);

        for(int i = 0; i < 5; i++) 
            printf(" %s", curr->languages[i]);
        printf("\n");

        curr = curr->next;
    }
}

/*
 * printMoviesInYear - prints all of the movies in a given year
 * @head: head of linked list of movies
 * @year: year to search for movies in
 */
void printMoviesInYear(struct movie *head, int year) {
    struct movie *curr = head;
    int nFound = 0;

    // go through list and print each movie from that year:
    while(curr != NULL) {
        if (curr->year == year) {
            printf("%s\n", curr->title);

            nFound++;
        }

        curr = curr->next;
    }
    
    if(nFound == 0) {
        printf("No data about movies released in the year %d\n", year);
    }
}

/*
 * printHighestRatedInEachYear - prints the highest rated movie in each year
 * @head: head of linked list of movies
 */
void printHighestRatedInEachYear(struct movie *head) {
    // for each year go through list and find the highest rated movie
    for(int year = 1900; year <= 2021; year++) {
        struct movie *curr = head;
        struct movie *topDog = NULL;

        // find the highest rated movie
        while(curr != NULL) {
            if(curr->year != year) {
                curr = curr->next;
                continue;
            }

            if(topDog == NULL) topDog = curr;
            if(curr->rating > topDog->rating) topDog = curr;

            curr = curr->next;
        }

        // print if a movie is found
        if(topDog != NULL) printf("%d %.1f %s\n", year, topDog->rating, topDog->title);
    }
}

/*
 * printMoviesInLanguage - prints all of the movies in a given language
 * @head: head of linked list of movies
 * @langueage: the language to look for movies in
 */
void printMoviesInLanguage(struct movie *head, char *language) {
    struct movie *curr = head;
    int movieFound = 0;

    // go through list and print each movie where language matches:
    while(curr != NULL) {
        // check if language matches any of curr's languages
        int langFound = 0;
        for(int i = 0; i < 5; i++) {
            if(strcmp(curr->languages[i], language) == 0) langFound = 1;
        }
        if(langFound == 0) {
            curr = curr->next;
            continue;
        }

        // if language matches
        printf("%d %s\n", curr->year, curr->title);

        movieFound = 1;
        curr = curr->next;
    }

    if(movieFound == 0) printf("No data about movies released in %s\n", language);
}

// function header template:
/*
 * brief_description_of_function - function's purpose
 * @param1: description of first input parameter
 * @param2: description of second input parameter
 * @param3: description of third input parameter
 *
 * Return: description of the return value
 */