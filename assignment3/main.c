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

        // continue if blank line or comment
        if(input[0] == '#') {
            continue;
        }

        // remove trailing endline
        input[strcspn(input, "\n")] = 0;

        char *token = strtok(input, " \n");
        if(token == NULL) continue;

        printf("command to exec: %s\n", input);
    }
}