#include "client.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in serverAddress;

    // Check usage & args
    if (argc < 4) { 
        fprintf(stderr,"USAGE: %s Plaintext Key Port\n", argv[0]); 
        exit(1); 
    }

    // Create a socket
    int socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    if (socketFD < 0) {
        error("dec_client: ERROR opening socket", 1);
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        fprintf(stderr, "dec_client: could not contact dec_server on port %d", atoi(argv[3]));
        exit(2);
    }

    char plainText[200000];
    char keyText[200000];
    char cipherText[200000];
    char buffer[256];

    // get file lengths and check their contents
    int cipherTextLen = processFile(argv[1], cipherText, "dec_client");
    int keyTextLen = processFile(argv[2], keyText, "dec_client");

    if (keyTextLen < cipherTextLen) {
        fprintf(stderr, "dec_client: ERROR key is too short\n");
        exit(1);
    }
    
    // authenticate with dec_server
    receive(buffer, socketFD);

    // error if wrong authentication code
    if (strcmp(buffer, "dec_server") != 0) {
        fprintf(stderr, "dec_client: cannot connect to server other than dec_server\n");
        fprintf(stderr, "dec_client: could not contact dec_server on port %d\n", atoi(argv[3]));
        exit(2);
    }

    // send hello to dec_server
    sendAll("hello dec_server@@", socketFD);

    // receive ciphertext request
    receive(buffer, socketFD);
    
    if (strcmp(buffer, "ciphertext") != 0) {
        fprintf(stderr, "dec_client: expected ciphertext request from dec_server\n");
        exit(2);
    }

    // send ciphertext
    sendAll(cipherText, socketFD);
    sendAll("@@", socketFD);

    // receive key request
    receive(buffer, socketFD);

    if (strcmp(buffer, "key") != 0) {
        fprintf(stderr, "dec_client: expected key request from dec_server\n");
        exit(2);
    }

    // send key
    sendAll(keyText, socketFD);
    sendAll("@@", socketFD);

    // receive plaintext
    receive(plainText, socketFD);

    printf("%s\n", plainText);

    // Close the socket
    close(socketFD);

    return 0;
}