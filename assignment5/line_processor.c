#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "buffer.h"

// create buffers
struct buffer **buffers, *buffer1;

void *input_thread(void *args) {
    char input[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        memset(input, '\0', LINE_SIZE);
        fgets(input, LINE_SIZE, stdin);

        if(strcmp(input, "STOP\n") == 0) {
            stop = 1;
        }

        put_buffer_line(buffer1, input);
    }

    return NULL;
}

void *output_thread(void *args) {
    char line[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        get_buffer_line(buffer1, line);
        printf("line: %s\n", line);

        if(strcmp(line, "STOP\n") == 0) {
            stop = 1;
        }
    }

    return NULL;
}


int main(int argc, char **argv) {
    // initialize global buffer array
    init_buffers(buffers);
    buffer1 = (struct buffer *) malloc(sizeof(struct buffer));
    buffer1->prod_idx = 0;
    buffer1->con_idx = 0;
    buffer1->count = 0;
    buffer1->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    buffer1->full = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

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