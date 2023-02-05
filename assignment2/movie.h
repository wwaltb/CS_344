#ifndef __MOVIE_H
#define __MOVIE_H

struct movie;

struct movie *createMovie(char *line);
struct movie *createMovieList(char *filename);
void freeMovieList(struct movie *head);
void processMovieFile(char *filename, struct movie *head);
void printMovies(struct movie *head);
void printMoviesInYear(struct movie *head, int year);
void printHighestRatedInEachYear(struct movie *head);
void printMoviesInLanguage(struct movie *head, char *language);

#endif