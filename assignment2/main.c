#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "movie.h"
#include "input.h"

/*
 * scanDirectory - scans the current directory for either the
 *      largest or smallest csv file with the prefix "movies_"
 * @filename: output string for the found file
 * @largest: flag that indicates whether to search for the
 *      largest or smallest csv file (1 or 0)
 */
void scanDirectory(char *filename, int largest) {
    DIR *dir = opendir(".");
    struct dirent *ent;
    struct stat st;

    int minmax = -1;

    while((ent = readdir(dir)) != NULL) {
        // only entries with prefix "movies_" and file extension ".csv"
        if(strncmp(ent->d_name, "movies_", 7) != 0 || strstr(ent->d_name, ".csv") == NULL) continue;

        // get matching meta data
        stat(ent->d_name, &st);
        
        // check if smallest or largest file
        if(largest) {
            if(minmax == -1 || st.st_size > minmax) {
                minmax = st.st_size;
                strcpy(filename, ent->d_name);
            }
        } else {
            if(minmax == -1 || st.st_size < minmax) {
                minmax = st.st_size;
                strcpy(filename, ent->d_name);
            }
        }
    }
    closedir(dir);
}

/*
 * findFile - prompts user until a movie file is found and 
 *      returns the file name and the head of a linked list 
 *      of movies from the file
 * @filename: output string for the specified file
 * @head: output var for head of movie linked list
 */
void findFile(char *filename, struct movie *head) {
    int found = 0;

    char *choiceStr = (char*) calloc(9, sizeof(char));
    int   choiceInt;
    
    while (!found) {
        // get user choice
        printf("Which file do you want to process?\n");
        printf("1. Process the largest file\n");
        printf("2. Process the smallest file\n");
        printf("3. Process specified file\n\n");
        getInput("Enter a choice 1 to 3: ", choiceStr);
        choiceInt = atoi(choiceStr);
        printf("\n");

        // get filename
        switch (choiceInt) {
        case 1:
            scanDirectory(filename, 1);
            break;
        case 2:
            scanDirectory(filename, 0);
            break;
        case 3:
            getInput("Enter the complete file name: ", filename);
            break;
        default:
            printf("Incorrect choice. Try again\n\n");
            continue;
        }

        // try to process file into linked list of movies
        head = createMovieList(filename);

        // prompt again if list can't be created
        if(head == NULL) {
            if(choiceInt == 3) printf("The file %s was not found. Try again\n\n", filename);
            else printf("Couldn't find a csv file with prefix \"movies_\". Try again\n\n");
            continue;
        }

        // file has been found and list has successfully been created
        found = 1;
    }

    free(choiceStr);
}

/*
 * processFile - handles option for when user wants to process a file
 */
void processFile() {
    char *filename = (char*) calloc(256, sizeof(char));
    struct movie *head;

    findFile(filename, head);

    printf("Now processing file %s\n", filename);
    head = createMovieList(filename);
    processMovieFile(filename, head);

    printf("\n");
    free(filename);
}

int main(int argc, char **argv) {
    int quit = 0;
    char *choiceStr = (char*) calloc(9, sizeof(char));
    int   choiceInt;

    while (!quit) {
        // get user choice
        printf("1. Select file to process\n");
        printf("2. Exit the program\n\n");
        getInput("Enter a choice 1 or 2: ", choiceStr);
        choiceInt = atoi(choiceStr);
        printf("\n");

        // process a file or quit
        switch (choiceInt) {
        case 1:
            processFile();
            break;
        case 2:
            quit = 1;
            break;
        default:
            printf("Incorrect choice. Try again\n\n");
            break;
        }
    }

    free(choiceStr);
}