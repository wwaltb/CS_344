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
        fprintf(stderr, "enc_client: ERROR key is too short");
        exit(1);
    }
    
    printf("plainTextLen: %da\n", plainTextLen);
    printf("keyTextLen: %d\n", keyTextLen);

    printf("plaintext: %sa\n", plainText);
    printf("key: %s\n", keyText);

    // Close the socket
    close(socketFD);

    return 0;
}