#include <stdbool.h>

/* -----------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * Header file for project3.c 
 * -----------------------------------------------------------------------*/

#ifndef FUNCTIONS
#define FUNCTIONS

int pages[250];  // Global array to save page numbers from the memory file.
int page_refs;  // Global counter to count how many page references are added to pages array.

// Page structure created to store both the page number and a dirty bit that will be set
// to either 0 or 1. 1 = page has been modified, and 0 = page unmodified.

typedef struct {
    int page_num;
    int dirty_bit;
} Page;

// Function implemented in readfile.c
void read_file();

// Functions implemented in fifo.c
void run_fifo();
void enqueue(int page_num);
void dequeue();
int isFull();
int isEmpty();
void display();
bool inQueue(int pagenum);
int queueIndex(int pagenum);

#endif