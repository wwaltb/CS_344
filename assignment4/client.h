#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()
#include <ctype.h>      // isupper()

// Error function used for reporting issues
void error(const char *message, int exitValue) { 
    perror(message); 
    exit(exitValue); 
} 

// Set up the address struct
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber, 
                        char* hostname) {
 
    // Clear out the address struct
    memset((char*) address, '\0', sizeof(*address)); 

    // The address should be network capable
    address->sin_family = AF_INET;
    // Store the port number
    address->sin_port = htons(portNumber);

    // Get the DNS entry for this host name
    struct hostent* hostInfo = gethostbyname(hostname); 
    if (hostInfo == NULL) { 
        fprintf(stderr, "CLIENT: ERROR, no such host\n"); 
        exit(0); 
    }
    // Copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy((char*) &address->sin_addr.s_addr, 
        hostInfo->h_addr_list[0],
        hostInfo->h_length);
}

int processFile(const char *filename, char *content) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        error("enc_client: ERROR opening input files", 1);
    }

    memset(content, '\0', sizeof(content));

    char currChar;
    int charsRead;

    while(1) {
        currChar = fgetc(file);

        if (currChar == EOF || currChar == '\n') {
            break;
        }

        if (!isupper(currChar) && currChar != ' ') {
            error("enc_client: ERROR input contains invalid characters", 1);
        }

        strcat(content, currChar);
        charsRead++;
    }

    fclose(file);

    return charsRead;
}