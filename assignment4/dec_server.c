#include "server.h"
#include "cipher.h"

int main(int argc, char *argv[]){
    struct sockaddr_in serverAddress;

    // Check usage & args
    if (argc < 2) { 
        fprintf(stderr,"USAGE: %s port\n", argv[0]); 
        exit(1);
    } 

    // Create the socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        error("dec_server: ERROR opening socket", 1);
    }

    // Set up the address struct for the server socket
    setupAddressStruct(&serverAddress, atoi(argv[1]));

    // Associate the socket to the port
    if (bind(listenSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        error("dec_server: ERROR on binding", 1);
    }

    // Start listening for connetions. Allow up to 5 connections to queue up
    listen(listenSocket, 5); 

    // Accept a connection, blocking if one is not available until one connects
    while(1) {
        int connectionSocket;
        struct sockaddr_in clientAddress;
        socklen_t sizeOfClientInfo = sizeof(clientAddress);

        // Accept the connection request which creates a connection socket
        connectionSocket = accept(listenSocket, (struct sockaddr *) &clientAddress, &sizeOfClientInfo); 
        if (connectionSocket < 0) {
            perror("dec_server: ERROR on accept");
        }

        // service client in a child process
        pid_t pid = fork();

        // error handling for fork()
        if (pid < 0) {
            perror("dec_server: ERROR on fork");
        }
        // child process
        else if (pid == 0) {
            int charsRead, charsSent;
            char plainText[200000];
            char keyText[200000];
            char cipherText[200000];
            char buffer[256];

            // send authentication
            sendAll("dec_server@@", connectionSocket);

            // receive hello from client
            receive(buffer, connectionSocket);

            // request ciphertext
            sendAll("ciphertext@@", connectionSocket);

            // receive ciphertext
            receive(cipherText, connectionSocket);

            // request key
            sendAll("key@@", connectionSocket);

            // receive key
            receive(keyText, connectionSocket);

            // decode plaintext
            decode(cipherText, keyText, plainText);

            // send plaintext
            sendAll(plainText, connectionSocket);
            sendAll("@@", connectionSocket);
        }
        // parent process
        else {

        }

        // Close the connection socket for this client
        close(connectionSocket);
    }

    // Close the listening socket
    close(listenSocket); 
    return 0;
}
