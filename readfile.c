#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */

/*
 * This function processes a comma separated value (CSV) file "memory_data.txt" and
 * reads it line by line, then uses the strtok() function to process the input.
 * The file should contain a list of "memory" addresses from a particular process,
 * separated by a comma. The page size is 4 KB, so only the first number of each 4-digit
 * address is saved into the pages array. The first digit represents the page number.
 * Preconditions: The function is called by project3.c with no parameters.
 * Postconditions: The function does error handling by priting an error to the console if
 *                 the file is not found.           
 */
void read_file() {
    FILE *read_ptr;  // Creates a file pointer object for reading the file.
    read_ptr = fopen("memory_data.txt", "r");
    
    // Checks if the file exists. If the file does not exist, it prints an
    // error and exits so that the program does not continue.
    if (read_ptr == NULL) {
        printf("Error. The file cannot be opened or does not exist.\n");
        exit(1);
    }
    
    char line[150];  // Temporarily store the lines from the file.
    int index = 0;

    // char *fgets(char *str, int n, FILE *stream) reads a line from the specified stream 
    // and stores it into the string pointed to by str, in this case "line".
    while(fgets(line, sizeof line, read_ptr) != NULL){
        // strtok() is a library function that breaks a string into a series of tokens
        // using a delimiter "char *strtok(char *str, const char *delim)".
        char* token = strtok(line, ",");
        // Keeps getting tokens while a comma is present in "line".
        while (token != NULL) {
            // We cast the string into an integer and divide by 1000 to get the first digit.
            int num = atoi(token) / 1000; 
            pages[index] = num; // Stores the integer into the pages array.
            token = strtok(NULL, ","); // Advances the tokenizer to the next comma separated string.
            index++; // Increments the pages array index.
        }
    }

    // Closes the file pointer objects.
    fclose(read_ptr);
}