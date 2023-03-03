#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main()
{
    int ret;
    int pipe_fd[2];
    ssize_t rlen, wlen;
    char *str = "Hello from the other side!";
    char *buf = (char *) malloc(100 * sizeof(char));
    
    if (pipe(pipe_fd) == -1) {
        perror("Error: failed to open a pipe.");
    }
    
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("Error: failed to fork()");

        case 0:
            rlen = read(pipe_fd[0], buf, 100);
            printf("[ Child | fd: %d]  read %s, len: %ld\n", pipe_fd[0], buf, rlen);
            close(pipe_fd[0]);
            break;

        default:
            wlen = write(pipe_fd[1], str, strlen(str)+1);
            printf("[Parent | fd: %d] write %s, len: %ld\n", pipe_fd[1], str, wlen);
            close(pipe_fd[1]);
            break;
    }

    return 0;
}