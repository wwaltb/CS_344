#ifndef __MOVIE_H
#define __MOVIE_H

struct movie;

struct movie *createMovie(char *line);
struct movie *processMoives(char *filename);

#endif