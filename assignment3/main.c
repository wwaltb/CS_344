#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// constants:
#define MAX_CHAR 2048
#define MAX_ARGS 512

// globals:
int fgOnlyMode;         // 0 = false, 1 = true

// forward declarations:
void loop();

int main() {
    loop();
}

void loop() {
    int status = 0;
    const char * devnull = "/dev/null";

    while(1) {
        char input[MAX_CHAR];
        char *argv[MAX_ARGS];
        int argc = 0;
        char *inputFile = NULL;
        char *outputFile = NULL;
        int runInBg = 0;

        // ignore SIGINT:
        struct sigaction SIGINT_action = {0};
        SIGINT_action.sa_handler = SIG_IGN;
        sigaction(SIGINT, &SIGINT_action, NULL);

        // check background processes:
        // TODO

        char *cwd = malloc(100);
        getcwd(cwd, 100);
        printf("cwd: %s\n", cwd);

        // prompt user for input:
        printf(": ");
		fflush(stdout);
		fflush(stdin);
        memset(input, '\0', MAX_CHAR);
		fgets(input, MAX_CHAR, stdin);

        if(input[0] == '#') continue;                       // continue if comment

        // remove trailing endline
        input[strcspn(input, "\n")] = 0;

        // expand variable '$$'

        // check for any non-whitespace chars
        char *token = NULL;
        if(!(token = strtok(input, " \n"))) continue;       // continue if blank line

        argv[0] = token;
        argc++;

        // parse input into command and arguments:
        char *prevToken = token;
        while(token = strtok(NULL, " ")) {

            // skip '<' and '>' characters
            if(strcmp(token, "<") == 0 || strcmp(token, ">") == 0) {
                prevToken = token;
                continue;
            }

            // handle special characters
            if(strcmp(prevToken, "<") == 0) {               // input file
                inputFile = token;
                prevToken = token;
                printf("inputFile: %s\n", inputFile);
                continue;
            }

            if(strcmp(prevToken, ">") == 0) {               // output file
                outputFile = token;
                prevToken = token;
                printf("outputFile: %s\n", outputFile);
                continue;
            }

            if(strcmp(token, "&") == 0) {                   // background mode
                runInBg = 1;
                break;
            }

            // assume token is argument otherwise
            argv[argc] = token;
            argc++;

            prevToken = token;                              // store token for next iteration
        }
        argv[argc] = NULL;          // null terminate argv

        // print arguments:
        int i = 0;
        while(argv[i] != NULL) {
            printf("%s ", argv[i]);
            i++;
        }
        printf("\n");

        // run command:

        // built in commands:
        if(strcmp(argv[0], "exit") == 0) {                  // exit command
            // kill all processes
            break;
        }

        if(strcmp(argv[0], "cd") == 0) {                    // cd command
            if(argc == 1) {
                // chdir to home, print error if fails
                if(chdir(getenv("HOME")) != 0) perror("cd");
            }
            else if(argc == 2) {
                // chdir to argv[1], print error if fails
                if(chdir(argv[1]) != 0) perror("cd");
            }
            else {
                printf("smallsh: cd: usage: cd [directory]\n");
                fflush(stdout);
            }
            continue;
        }

        if(strcmp(argv[0], "status") == 0) {                // status command
            if (WIFEXITED(status)){
				printf("exit value %d\n", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)){
				printf("terminated by signal %d\n", WTERMSIG(status));
			}
            fflush(stdout);
            continue;
        }

        // use exec() to run command:
        pid_t pid = fork();
        int fdIn, fdOut;

        if(pid == -1) {                                     // fork error
            perror("fork");
            exit(1);
        }

        else if(pid == 0) {                                 // child process
            if(!runInBg) {
                SIGINT_action.sa_handler = SIG_DFL;
                SIGINT_action.sa_flags = 0;
                sigaction(SIGINT, &SIGINT_action, NULL);
            }

            if(runInBg && !inputFile) inputFile = devnull;
            if(runInBg && !outputFile) outputFile = devnull;

            if(inputFile) {
                fdIn = open(inputFile, O_RDONLY);
                if(fdIn == -1) {
                    perror("open");
                    exit(1);
                }
                dup2(fdIn, STDIN_FILENO);
            }

            if(outputFile) {
                fdOut = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                if(fdOut == -1) {
                    perror("open");
                    exit(1);
                }
                dup2(fdOut, STDOUT_FILENO);
            }

            status = execvp(argv[0], argv);
            if(status == -1) {
                perror(argv[0]);
                status = 1;
            }
            exit(EXIT_FAILURE);
        }

        else {                                              // parent process
            if(runInBg) {
                continue;
            }

            waitpid(pid, &status, 0);

            if(WIFSIGNALED(status)) {
                status = WTERMSIG(status);
                printf("terminated by signal %d\n", status);
            }
            else if(WIFEXITED(status)) {
                status = WEXITSTATUS(status);
            }
        }
    }
}