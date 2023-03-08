#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error function used for reporting issues
void error(const char *message, int exitValue) { 
  perror(message); 
  exit(exitValue); 
}

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, int portNumber) {
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);
    // Allow a client at any address to connect to this server
    address->sin_addr.s_addr = INADDR_ANY;
}

/*
 * receives data from a client
 * @string: string buffer to store the data
 * @socket: socket fd to receive data from
 *
 * Return: 0 on success, -1 on failure
 */
int receive(char *string, int socket) {
    memset(string, '\0', sizeof(string));

    char buffer[256];
    int charsRead;

    while(1) {
        memset(buffer, '\0', 256);
        charsRead = recv(socket, buffer, 255, 0);

        // handle recv error
        if (charsRead < 0) {
            error("ERROR reading from socket", 1);
            break;
        }

        printf("SERVER received buffer: %s\n", buffer);

        // check if end of data from client
        if (strcmp(buffer, "@@") == 0) {
            break;
        }

        // append data to string
        strcat(string, buffer);
    }

    // return 0 on success, -1 if error
    return charsRead < 0 ? -1 : 0;
}

/*
 * sends data to a client
 * @string: string to send
 * @socket: socket fd to send data to
 *
 * Return: # of chars sent on success, -1 on failure
 */
int sendAll(char *string, int socket) {
    int charsSent = 0, charsLeft = strlen(string), n;

    while(charsSent < strlen(string)) {
        n = send(socket, string + charsSent, charsLeft, 0);
        if (n < 0) {
            perror("ERROR writing to socket");
            break;
        }

        charsSent += n;
        charsLeft -= n;
    }

    // return charsSent on success, -1 if error
    return n < 0 ? -1 : charsSent;
}