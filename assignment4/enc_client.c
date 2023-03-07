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
        error("enc_client: ERROR opening socket", 1);
    }

    // Set up the server address struct
    setupAddressStruct(&serverAddress, atoi(argv[3]), "localhost");

    // Connect to server
    if (connect(socketFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
        error("enc_client: ERROR connecting", 1);
    }

    // create file pointers and open plaintext and key
    char plainText[200000];
    char keyText[200000];
    char cipherText[200000];

    int plainTextLen = processFile(argv[1], plainText);
    int keyTextLen = processFile(argv[2], keyText);

    if (keyTextLen < plainTextLen) {
        fprintf(stderr, "enc_client: ERROR key is too short\n");
        exit(1);
    }
    
    // authenticate with enc_server
    char authCode[10];
    receive(authCode, socketFD);

    if (strcmp(authCode, "enc_server") != 0) {
        fprintf(stderr, "enc_client: cannot connect to server other than enc_server\n");
        fprintf(stderr, "enc_client: could not contact enc_server on port %d", atoi(argv[3]));
        exit(2);
    }

    // send plaintext
    sendAll(plainText, socketFD);
    sendAll("@exit", socketFD);

    // send key
    sendAll(keyText, socketFD);
    sendAll("@exit", socketFD);

    // receive ciphertext
    receive(cipherText, socketFD);

    printf("%s\n", cipherText);

    // Close the socket
    close(socketFD);

    return 0;
}