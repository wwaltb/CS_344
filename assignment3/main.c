#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

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
    int exit = 0;
    int status = 0;
    const char * devnull = "/dev/null";

    while(!exit) {
        char input[2048];
        char *argv[512];
        int argc = 0;
        char *inputFile;
        char *outputFile;
        int runInBg = 0;

        // prompt user for input:
        printf(": ");
		fflush(stdout);
		fflush(stdin);
        memset(input, '\0', MAX_CHAR);
		fgets(input, MAX_CHAR, stdin);

        if(input[0] == '#') continue;                       // continue if comment

        // remove trailing endline
        input[strcspn(input, "\n")] = 0;

        // check for any non-whitespace chars
        char *token = NULL;
        if(!(token = strtok(input, " \n"))) continue;       // continue if blank line
        printf("token: %s\n", token);

        argv[0] = token;
        argc++;

        // parse input into command and arguments:
        char *prevToken = token;
        while(token = strtok(NULL, " ")) {
            printf("token: %s\n", token);
            printf("prevToken: %s\n", prevToken);

            if(strcmp(prevToken, "<") == 0) {               // input file
                inputFile = token;
                continue;
            }

            if(strcmp(prevToken, ">") == 0) {               // output file
                outputFile = token;
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
    }
}