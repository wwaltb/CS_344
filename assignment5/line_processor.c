#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "buffer.h"

// create buffers array
struct buffer **buffers;

// input thread
void *input_thread(void *args) {
    char input[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        memset(input, '\0', LINE_SIZE);
        fgets(input, LINE_SIZE, stdin);

        if(strcmp(input, "STOP\n") == 0) {
            stop = 1;
        }

        put_buffer_line(buffers[0], input);
    }

    return NULL;
}

// line seperator thread
void *line_seperator_thread(void *args) {
    char line[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        get_buffer_line(buffers[0], line);

        if(strcmp(line, "STOP\n") == 0) {
            stop = 1;
        }

        // loop through line characters
        int i;
        for(i = 0; i < strlen(line); i++) {
            // replace newlines ('\n') with a space (' ')
            if(line[i] == '\n') line[i] = ' ';
        }

        put_buffer_line(buffers[1], line);
    }
}

// output thread
void *output_thread(void *args) {
    char line[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        get_buffer_line(buffers[1], line);
        printf("line: %s\n", line);

        if(strcmp(line, "STOP ") == 0) {
            stop = 1;
        }
    }

    return NULL;
}


int main(int argc, char **argv) {
    // initialize global buffer array
    buffers = (struct buffer **) malloc(sizeof(struct buffer *) * NUM_BUFFERS);

    int i;
    for(i = 0; i < NUM_BUFFERS; i++) {
        buffers[i] = (struct buffer *) malloc(sizeof(struct buffer));
        buffers[i]->prod_idx = 0;
        buffers[i]->con_idx = 0;
        buffers[i]->count = 0;
        pthread_mutex_init(&buffers[i]->mutex, NULL);
        pthread_cond_init(&buffers[i]->full, NULL);
    }

    // create threads
    pthread_t input_t, line_separator_t, output_t;
    pthread_create(&input_t, NULL, input_thread, NULL);
    pthread_create(&line_separator_t, NULL, line_seperator_thread, NULL);
    pthread_create(&output_t, NULL, output_thread, NULL);

    // wait for threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(line_separator_t, NULL);
    pthread_join(output_t, NULL);

    // free global buffer array
    for(i = 0; i < NUM_BUFFERS; i++) {
        free(buffers[i]);
    }
    free(buffers);

    return(0);
}