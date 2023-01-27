# build an executable named movies from assignment1.c and movie.c
CC = gcc
CFLAGS = -std=gnu99

all: assignment1.c movie.c
	${CC} ${CFLAGS} -o movies assignment1.c movie.c

clean:
	rm movies

test:
	${CC} ${CFLAGS} -o movies assignment1.c movie.c
	./movies movies_sample_1.csv

leak-test:
	${CC} ${CFLAGS} -o movies assignment1.c movie.c
	valgrind --leak-check=yes ./movies movies_sample_1.csv