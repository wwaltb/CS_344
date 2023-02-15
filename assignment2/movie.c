#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

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
 * createMovieList - processes a movie data file into a linked 
 *      list of movies
 * @filename: full name of the movie file
 *
 * Return: the head of the linked list of movies
 */
struct movie *createMovieList(char *filename) {
    // open filename for reading
    FILE *moviesFile = fopen(filename, "r");
    if(moviesFile == NULL) {
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

    return head;
}

/*
 * freeMovieList - frees a list of movies
 * @head: the head of the linked list of movies
 */
void freeMovieList(struct movie *head) {
    if(head == NULL) return;
    freeMovieList(head->next);
    free(head);
}

/*
 * processMovieFile - makes a file of movies for each year
 *      from a movie data file. This file is put into a new
 *      random directory.
 * @filename: string containing the file name containing the
 *      movie data.
 * @head: the head of the linked list of movies
 */
void processMovieFile(char *filename, struct movie *head) {
    srand(time(NULL));          // initialize random seed

    // make a directory named "bringenw.movie.random":
    char *dirName = (char*) calloc(strlen("bringenw.movies.random"), sizeof(char));
    sprintf(dirName, "bringenw.movies.%d", rand() % 100000);      // .random is random [0,99999]

    // successfully create a new directory
    int result = 1;
    while(result != 0) {
        result = mkdir(dirName, S_IRWXU | S_IRGRP | S_IXGRP);     // permissions 'rwxr-x---'
        printf("Created directory with name %s\n", dirName);
    }

    // open directory and make file containing movies for each year
    DIR *dir = opendir(dirName);

    printf("opened dir");   // delete me later

    for(int year = 1900; year <= 2021; year++) {            // go through each year
        printf("year %d\n", year);  // delete me later
        char filename[strlen(dirName) + strlen("xxxx.txt") + 2];
        sprintf(filename, "%s/%d.txt", dirName, year);                  // string contains "year.txt"
        FILE *file = NULL;

        struct movie *curr = head;
        while(curr != NULL) {
            // continue if movie is not in year
            if(curr->year != year) {
                curr = curr->next;
                continue;
            }

            // create file if first movie found
            if(file == NULL) {
                file = fopen(filename, "w");
                chmod(filename, S_IRUSR | S_IWUSR | S_IRGRP);           // permissions 'rw-r-----'
            }

            // add movie title to a new line
            fprintf(file, "%s\n", curr->title);

            curr = curr->next;
        }
        fclose(file);
    }

    closedir(dir);
    free(dirName);
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