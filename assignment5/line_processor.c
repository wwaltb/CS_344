#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "buffer.h"

// create buffers
struct buffer **buffers;

void *input_thread(void *args) {
    char input[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        fgets(input, LINE_SIZE, stdin);

        if(strcmp(input, "STOP\n")) {
            stop = 1;
        }

        put_buffer_line(buffers[0], input);
    }

    return NULL;
}

void *output_thread(void *args) {
    char line[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        get_buffer_line(buffers[0], line);
        printf("line: %s\n", line);
    }

    return NULL;
}


int main(int argc, char **argv) {
    // initialize global buffer array
    init_buffers(buffers);

    printf("buffers initialized\n");

    // create threads
    pthread_t input_t, output_t;
    pthread_create(&input_t, NULL, input_thread, NULL);
    pthread_create(&output_t, NULL, output_thread, NULL);

    printf("threads created\n");

    // wait for threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(output_t, NULL);

    printf("threads terminated\n");

    // free global buffer array
    free(buffers);

    printf("buffers freed\n");

    return(0);
}