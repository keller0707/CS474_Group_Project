#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */
#define NFRAMES 5

Page queue[NFRAMES]; // We create a queue to simulate a 10 page frame structure.
int head = -1, tail = -1;

int pageFault = 0;     // This counter will keep track of how many page faults occur.
int pg_index = 0;   // This variable will control the index of the page numbers array.
int rw_counter = 0;  // The read/write counter.

/*
 * This function uses the page numbers from the pages array to load up the queue that
 * represents the page frames. It calls the function inQueue() to check if the page is
 * already loaded in the frame, and if it isn't, it calls enqueue() to load it. If the
 * the page is already in the frame, then it sets to the dirty bit to "true".
 * Preconditions: The function to read the memory file has to be called by main first, 
 *                before calling this function, because the array pages[] has to exist.
 * Postconditions: The function prints how many page faults occurred while loading the
 *                 pages into the frames, and also how many read/writes to disk occurred.
 */
void run_fifo() {
    // Loop runs while index hasn't reached the size of the pages array.
    while (pg_index != page_refs) {
        // Checks if the page is not in the page frame (queue) yet.
        if (inQueue(pages[pg_index]) == false) {
            // Enqueue the page and add it to a frame.
            enqueue(pages[pg_index]);
            // Because it's not in the queue, it's a page fault so we increment the fault counter.
            pageFault++;
        }
        else {
            // It's already in the queue and we need to find the page to get its index.
            int q_idx = queueIndex(pages[pg_index]);
            // Uses the index to set the dirty bit to 1 (true) because memory is accessing 
            // the page again to modify it. 
            queue[q_idx].dirty_bit = 1;
        }
        pg_index++;
    }
    printf("\nThe system had %d page faults during the process execution.\n", pageFault);
    printf("There were %d read/write actions to disk during the process execution.\n", rw_counter);
}

/*
 * This function adds the page into the pages frame (queue). It checks of the queue 
 * is full and if it isn't then inserts the page at the end of the queue.
 * Preconditions: The function must be called with the page number that is to be added.
 * Postcondtions: None.
 */
void enqueue(int pg_num) {
    // First we check if the frames are full.
    if (isFull()) {
        dequeue(); // Call dequeue to first delete a page from the frame.
        enqueue(pg_num);  // Add the page to the newly open frame.
    }
    else {
        if (head == -1) {
            head = 0;
        }
        tail = (tail + 1) % NFRAMES;
        queue[tail].page_num = pg_num;
        // If we are enqueueing a page into the page frame, then it has not been modified
        // by the system and we need to set the dirty bit to 0, which is "false".
        queue[tail].dirty_bit = 0;
        // Uncomment for debugging.
        //printf("Added to queue -> %d\n", queue[tail].page_num);
    }
}

/*
 * This function deletes a page from the pages frame (queue). First it checks if 
 * the queue is empty, and if it's empty it prints an error. Otherwise it advances 
 * the head pointer to the next spot in the queue.
 * Preconditions: None
 * Postconditions: None
 */
void dequeue() {
    //int pg; // Uncomment for debugging.
    // First checks if the queue is empty.
    if (isEmpty()) {
        printf("Error! Queue is empty. Unable to delete items.\n");
    }
    else {
        // Checks if the page has the dirty bit set and was modified.
        if (queue[head].dirty_bit == 1) {
            rw_counter++;  // Increments the read/write counter.
        }
        //pg = queue[head].page_num;  // Uncomment for debugging.
        if (head == tail) {
            head = -1;
            tail = -1;
        }
        // Advances head on queue after page is removed from frame.
        else {
            head = (head + 1) % NFRAMES;
        }
        // Uncomment for debugging.
        //printf("Removed -> %d\n", pg);
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
            printf("%d\t", queue[i].page_num);
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
        if(queue[i].page_num == pagenum) {
            return true;
        }
    }
    return false;
}

/*
 * This function checks if a page number is already in the queue, and returns the
 * index where it is found.
 * Preconditions: The function needs as a parameter the page number to search.
 * Postconditions: The function returns the index of where the page number is found.
 */
int queueIndex(int pagenum) {
    for (int i = 0; i < NFRAMES; i++) {
        if(queue[i].page_num == pagenum) {
            return i;
        }
    }
    return (-1);
}

