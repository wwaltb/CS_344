compile using make or by running the command:
    gcc -std=gnu99 -o movies_by_year main.c movie.c

makefile has the commands:
    clean     - remove all files from make
    test      - run make and run the executable
    leak-test - run make and run the executable using valgrind