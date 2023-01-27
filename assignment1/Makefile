movies: assignment1.c movie.c
	gcc assignment1.c movie.c -o movies

# build an executable named movies from main.c
all: main.c 
    gcc --std=gnu99 -o movies assignment1.c movie.c

clean:
    rm movies

test:
    gcc --std=gnu99 -o movies assignment1.c movie.c
    ./movies movies_sample_1.csv

leak-test:
    gcc --std=gnu99 -o movies assignment1.c movie.c
    valgrind --leak-check=yes ./movies movies_sample_1.csv