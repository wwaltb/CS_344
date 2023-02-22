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
void checkBgProcesses();
void killBgProcesses();
char* expand$$(char*);

int main() {
    int status = 0;
    char *devnull = "/dev/null";

    while(1) {
        char input[MAX_CHAR];           // the input string
        char command[MAX_CHAR * 2];     // the command after var '$$' expansion
        char *argv[MAX_ARGS];           // list of arguments
        int argc = 0;                   // number of arguments
        char *inputFile = NULL;         // file to redirect input
        char *outputFile = NULL;        // file to redirect output
        int runInBg = 0;

        // ignore SIGINT:
        struct sigaction SIGINT_action = {0};
        SIGINT_action.sa_handler = SIG_IGN;
        sigaction(SIGINT, &SIGINT_action, NULL);

        // check background processes:
        checkBgProcesses();

        // prompt user for input:
        printf(": ");
		fflush(stdout);
		fflush(stdin);
        memset(input, '\0', strlen(input));
		fgets(input, MAX_CHAR, stdin);

        if(input[0] == '#') continue;                       // continue if comment

        // remove trailing endline
        input[strcspn(input, "\n")] = 0;

        // expand variable '$$'
        memset(command, '\0', strlen(input));
        strncpy(command, expand$$(input), strlen(expand$$(input)));

        // check for any non-whitespace chars
        char *token = NULL;
        if(!(token = strtok(command, " \n"))) continue;     // continue if blank line

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
                continue;
            }

            if(strcmp(prevToken, ">") == 0) {               // output file
                outputFile = token;
                prevToken = token;
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

        // run command:

        // built in commands:
        if(strcmp(argv[0], "exit") == 0) {                  // exit command
            killBgProcesses();
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
            if (WIFEXITED(status)) {
				printf("exit value %d\n", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)) {
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
            // catch SIGIN with default behavior for child processes
            if(!runInBg) {
                SIGINT_action.sa_handler = SIG_DFL;
                SIGINT_action.sa_flags = 0;
                sigaction(SIGINT, &SIGINT_action, NULL);
            }

            // set background redirects if not specified
            if(runInBg && !inputFile) inputFile = devnull;
            if(runInBg && !outputFile) outputFile = devnull;

            // redirect input
            if(inputFile) {
                fdIn = open(inputFile, O_RDONLY);
                if(fdIn == -1) {
                    printf("cannot open %s for input\n", inputFile);
                    fflush(stdout);
                    exit(1);
                }
                dup2(fdIn, STDIN_FILENO);
            }

            // redirect output
            if(outputFile) {
                fdOut = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                if(fdOut == -1) {
                    printf("cannot open or create %s for output\n", inputFile);
                    fflush(stdout);
                    exit(1);
                }
                dup2(fdOut, STDOUT_FILENO);
            }

            status = execvp(argv[0], argv);
            if(status == -1) {
                perror(argv[0]);
                exit(1);
            }
            exit(0);
        }

        else {                                              // parent process
            if(runInBg) {
                printf("background pid is %d\n", pid);
                continue;
            }

            waitpid(pid, &status, 0);

            if(WIFSIGNALED(status)) {
                printf("terminated by signal %d\n", WTERMSIG(status));
                fflush(stdout);
            }
        }
    }
}

void checkBgProcesses() {
    pid_t pid;
    int status;
    do {
        pid = waitpid(-1, &status, WNOHANG);
        if(pid == 0 || pid == -1) continue;
        // child process has exited:
        if(WIFEXITED(status)) {
            printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(status));
            fflush(stdout);
        }
        else if(WIFSIGNALED(status)) {
            printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(status));
            fflush(stdout);
        }
    } while(pid != 0 && pid != -1);
}

void killBgProcesses() {
    pid_t pgid = getpgid(0);
    kill(-pgid, SIGKILL);
}

char* expand$$(char *string) {
    int i = 0;
    int j = 0;

    // make a string containing the process id:
    pid_t pid = getpid();
    int pidLen = snprintf(NULL, 0, "%d", pid);
    char pidStr[pidLen + 1];
    snprintf(pidStr, pidLen + 1, "%d", pid);

    char *expandedString = malloc(strlen(string) * 2);

    while(i < strlen(string)) {
        if(string[i] == '$' && string[i+1] == '$') {
            // replace "$$" with pid
            strncpy(&expandedString[j], pidStr, pidLen);
            i += 2;
            j += pidLen;
        }
        else {
            // add current char
            expandedString[j] = string[i];
            i++;
            j++;
        }
    }

    return expandedString;
}

// TODO:
// handle ctrl-z
// fix status messages