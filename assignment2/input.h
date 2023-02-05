#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * getInput - prompts the user with a message and returns the input string
 * @message: message to prompt user with
 * @input: string to store user input
 */
void getInput(char *message, char *input) {
    memset(input, 0, strlen(input));        // reset input string
    printf("%s", message);                  // print message
    scanf("%s", input);                     // get input
    input[strcspn(input, "\n")] = 0;        // remove trailing \n
}