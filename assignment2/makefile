# build an executable named movies from assignment1.c and movie.c
CC = gcc
CFLAGS = -std=gnu99 -g

all: main.c movie.c
	${CC} ${CFLAGS} -o movies_by_year main.c movie.c

clean:
	rm movies

test:
	${CC} ${CFLAGS} -o movies_by_year main.c movie.c
	./movies_by_year

leak-test:
	${CC} ${CFLAGS} -o movies_by_year main.c movie.c
	valgrind --leak-check=yes ./movies_by_year