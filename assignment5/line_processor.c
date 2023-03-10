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
        memset(line, '\0', LINE_SIZE);
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

// plus sign thread
void *plus_sign_thread(void *args) {
    char line[LINE_SIZE];
    char replaced_line[LINE_SIZE];

    int stop = 0;
    while(!stop) {
        memset(line, '\0', LINE_SIZE);
        get_buffer_line(buffers[1], line);

        if(strcmp(line, "STOP ") == 0) {
            stop = 1;
        }

        // loop through line characters
        int i = 0;      // line iterator
        int j = 0;      // replaced_line iterator
        while(i < strlen(line)) {
            // replace "++" with "^"
            if(line[i] == '+' && line[i+1] == '+') {
                // add '^' char to replaced_line
                replaced_line[j] = '^';
                i += 2;
                j += 1;
            }
            // don't replace
            else {
                // add line char to replaced_line
                replaced_line[j] = line[i];
                i++;
                j++;
            }
        }

        put_buffer_line(buffers[2], replaced_line);
    }
}

// output thread
void *output_thread(void *args) {
    char buffer[MAX_LINES * LINE_SIZE];
    char line[LINE_SIZE];

    while(1) {
        memset(line, '\0', LINE_SIZE);
        get_buffer_line(buffers[2], line);

        if(strcmp(line, "STOP ") == 0) {
            break;
        }

        strcat(buffer, line);

        // print line if buffer is at least 80 chars
        size_t len = strlen(buffer);
        if(len >= 80) {
            printf("%.80s\n", buffer);
            memmove(buffer, buffer + 80, len - 80 + 1);
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
    pthread_t input_t, line_separator_t, plus_sign_t, output_t;
    pthread_create(&input_t, NULL, input_thread, NULL);
    pthread_create(&line_separator_t, NULL, line_seperator_thread, NULL);
    pthread_create(&plus_sign_t, NULL, plus_sign_thread, NULL);
    pthread_create(&output_t, NULL, output_thread, NULL);

    // wait for threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(line_separator_t, NULL);
    pthread_join(plus_sign_t, NULL);
    pthread_join(output_t, NULL);

    // free global buffer array
    for(i = 0; i < NUM_BUFFERS; i++) {
        free(buffers[i]);
    }
    free(buffers);

    return(0);
}