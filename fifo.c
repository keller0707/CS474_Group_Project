#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */
#define NFRAMES 10

int queue[NFRAMES]; // We create a queue to simulate a 10-frame structure.
int head = -1, tail = -1;
int pageFault = 0;     // This counter will keep track of how many page faults occur.
pg_index = 0;   // This variable will control the index of the pages array.

/*
 * This function uses the page numbers in the pages array to load up the frames queue.
 *
 * Preconditions: The function to read the memory file has to be called before calling
 *                this function, because the array pages[] already has to be filled.
 * Postconditions:
 */
void run_fifo() {
    // Check that the entry in the pages array doesn't equal to zero.
    while (pages[pg_index] != 0) {
        // Check if the page is not in the frame yet
        if (inQueue(pages[pg_index]) == false) {
            // This is a page fault so we increment the fault counter.
            pageFault++;
            // Enqueue the page and add it to a frame.
            enqueue(pages[pg_index]);
        }
        else {
            // Implement dirty bit here.
            printf(" ");
        }
        pg_index++;
    }

}

/*
 * This function adds the page into the frame. It checks of the queue is full
 * and if it isn't then inserts the page at the end of the queue.
 * Preconditions: The function must be called with the page that is to be added.
 * Postcondtions: None.
 */
void enqueue(int page_num) {
    // First we check if the frames are full.
    if (isFull()) {
        dequeue(); // Call dequeue to first delete a frame.
        enqueue(page_num);  // Add the page to the newly open frame.
    }
    else {
        if (head == -1) {
            head = 0;
        }
        tail = (tail + 1) % NFRAMES;
        queue[tail] = page_num;
        printf("Added to queue -> %d\n", queue[tail]);
    }
}

/*
 * This function deletes a page from a frame. First it checks if the queue is
 * empty, and if it's empty it prints an error. Otherwise it advances the head
 * pointer to the next spot in the queue.
 * Preconditions: None
 * Postconditions: None
 */
void dequeue() {
    int pg;
    // First check if the queue is empty.
    if (isEmpty()) {
        printf("Error! Queue is empty. Unable to delete items.\n");
    }
    else {
        pg = queue[head];
        if (head == tail) {
            head = -1;
            tail = -1;
        }
        // Advances head on queue after page is removed from frame.
        else {
            head = (head + 1) % NFRAMES;
        }
        printf("Removed -> %d\n", pg);
    }

}

/*
 * This function checks if the queue is full.
 * Preconditions: None
 * Posconditions: The function returns a 1 if the queue is full, otherwise it returns a 0.
 */
int isFull() {
    if ((head == tail + 1) || (head == 0 && tail == NFRAMES - 1)) {
        return 1;
    }
    return 0;
}

/*
 * This function checks if the queue is empty.
 * Preconditions: None
 * Postconditions: The function returns a 1 if the queue is empty, otherwise it returns a 0.
 */
int isEmpty() {
    if (head == -1) {
        return 1;
    } 
    return 0;
}

/*
 * This function prints the queue.
 * Preconditions: None
 * Postconditions: It prints the queue to the console.
 */
void display() {
    if (isEmpty()) {
        printf("The queue is empty.\n");
    }
    else {
        for (int i = 0; i < NFRAMES; i++) {
            printf("%d\t", queue[i]);
        }
        printf("\n");
        printf("Head = %d, Tail = %d\n", head, tail);
    }
}

/*
 * This function checks if a page number is already in the queue, and returns true
 * if it finds it. Else it returns false.
 * Preconditions: The function needs as a parameter the page number to search.
 * Postconditions: The function returns true if it finds the number, false otherwise.
 */
bool inQueue(int pagenum) {
    for (int i = 0; i < NFRAMES; i++) {
        if(queue[i] == pagenum) {
            return true;
        }
        return false;
    }
}

