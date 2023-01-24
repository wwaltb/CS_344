#ifndef __MOVIE_H
#define __MOVIE_H

struct movie;

struct movie *createMovie(char *line);
struct movie *processMovies(char *filename);

#endif