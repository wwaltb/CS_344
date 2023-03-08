#include <string.h>

/*
 * brief_description_of_function - function's purpose
 * @param1: description of first input parameter
 * @param2: description of second input parameter
 * @param3: description of third input parameter
 *
 * Return: description of the return value
 */
void templateFunction() {

}

/*
 * convert an integer to a char
 * @num: integer to convert (must be in range [0, 26])
 *
 * Return: char that is either a space (' ') or in the range [A,Z]
 */
char toChar(int num) {
    if(num == 26) return 32;
    return num + 65;
}

/*
 * convert a char to an integer
 * @c: char to convert (must be either a space (' ') or in the range [A,Z])
 *
 * Return: int that is in range [0, 26]
 */
int toInt(char c) {
    if(c == 32) return 26;
    return c - 65;
}

/*
 * encodes plaintext into ciphertext using the given key
 * @plainText: string of plaintext to encode
 * @keyText: string of cipher key
 * @cipherText: output string for encoded plaintext
 */
void encode(char *plainText, char *keyText, char *cipherText) {
    memset(cipherText, '\0', sizeof(cipherText));

    int i;
    for(i = 0; plainText[i] != '\0'; i++) {
        int addKey = (toInt(plainText[i]) + toInt(keyText[i])) % 27;
        cipherText[i] = toChar(addKey);
    }
}

/*
 * decodes ciphertext into plaintext using the given key
 * @cipherText: output string for encoded plaintext
 * @plainText: string of plaintext to encode
 * @keyText: string of cipher key
 */
void decode(char *cipherText, char *keyText, char *plainText) {
    memset(plainText, '\0', sizeof(plainText));

    int i;
    for(i = 0; cipherText[i] != '\0'; i++) {
        int subKey = toInt(cipherText[i]) - toInt(keyText[i]);
        subKey = subKey < 0 ? (subKey + 27) % 27 : subKey % 27;
        plainText[i] = toChar(subKey);
    }
}