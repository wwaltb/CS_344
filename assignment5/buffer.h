#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINES 50
#define LINE_SIZE 1000
#define NUM_BUFFERS 3

// struct to hold buffer and associated variables
struct buffer {
    char buffer[MAX_LINES][LINE_SIZE];
    int prod_idx;
    int con_idx;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t full;
};

// function for initializing array of buffers
struct buffer **init_buffers() {
    struct buffer **buffers = (struct buffer **) malloc(sizeof(struct buffer *) * NUM_BUFFERS);

    int i;
    for(i = 0; i < NUM_BUFFERS; i++) {
        buffers[i] = (struct buffer *) malloc(sizeof(struct buffer));
        buffers[i]->prod_idx = 0;
        buffers[i]->con_idx = 0;
        buffers[i]->count = 0;
        pthread_mutex_init(&buffers[i]->mutex, NULL);
        pthread_cond_init(&buffers[i]->full, NULL);
    }
}

// function for freeing array of buffers
void free_buffers(struct buffer **buffers) {
    int i;
    for(i = 0; i < NUM_BUFFERS; i++) {
        free(buffers[i]);
    }
    free(buffers);
}

// function to put a line into the buffer
void put_buffer_line(struct buffer *buffer, char *line) {
    // lock mutex before putting item into buffer
    pthread_mutex_lock(&buffer->mutex);

    // put item into buffer
    strcpy(buffer->buffer[buffer->prod_idx], line);

    // increment producer index and count
    buffer->prod_idx++;
    buffer->count++;

    // signal to consumer that buffer isn't empty
    pthread_cond_signal(&buffer->full);

    // unlock mutex
    pthread_mutex_unlock(&buffer->mutex);
}

// function to get a line from the buffer
void get_buffer_line(struct buffer *buffer, char *line) {
    // lock mutex before checking if line has data
    pthread_mutex_lock(&buffer->mutex);

    // wait if line is empty
    while(buffer->count == 0) {
        pthread_cond_wait(&buffer->full, &buffer->mutex);
    }

    // put buffer into output
    strcpy(line, buffer->buffer[buffer->con_idx]);

    // increment consumer index and decrement count
    buffer->con_idx++;
    buffer->count--;

    // unlock mutex
    pthread_mutex_unlock(&buffer->mutex);
}