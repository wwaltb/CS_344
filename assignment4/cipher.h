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