#include <stdio.h>
#include <stdbool.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */
#define NFRAME 5

Page table[NFRAME]; // We create a table to simulate a 10 page frame structure.
int table_cnt = 0;  // Helps track if the table is empty or full.

int page_frequencies[11]; // Array to store the frequencies of each page.

/*
 * This function simulates how an LFU (least frequently used) algorithm replaces pages in
 * the page frame. It loops through the pages array that contains the page numbers to be
 * added to the page table, and it checks if the pages are not yet in the frame
 * then calls the function to add them. It works with the other functions to mark a page
 * for deletion if the table is full. A page gets deleted according to the lowest
 * frequency that is tracked with the array page_frequencies.
 * Preconditions: To run this function, project3.c main() has to call the function to
 *                open and read the file containing the page numbers.
 * Postconditions: Prints the number of page faults that occurred during program execution
 *                 and the number of times a page was written to disk, then exits.
 */
void run_lfu() {
    // Initializes all values in the table to -1 to indicate the table is empty.
    for (int i = 0; i < NFRAME; i++) {
        table[i].page_num = -1;
    }

    // Loop runs while index hasn't reached the size of the pages array.
    while (pg_index != page_refs) {
        // Checks if the page is not in the page frame (table) yet.
        if (inTable(pages[pg_index]) == false) {
            // Calls the function to add the page to the page table.
            insertPage(pages[pg_index]);
            // Because it's not in the table, it's a page fault so we increment the fault counter.
            pageFault++;
            // Since a page was inserted into the table, the frequency of that page number has to
            // be updated in the array that tracks how often each page appears. The page number
            // itself is used as the array index, and what is stored in the array is the frequency.
            page_frequencies[pages[pg_index]] += 1;  
        }
        else {
            // Page is already in the table and we need to find the page to get its index.
            int table_idx = table_index(pages[pg_index]);
            // Uses the index to set the dirty bit to 1 (true) because memory is accessing 
            // the page again to modify it. 
            table[table_idx].dirty_bit = 1;
            // Since a page that is already in the table was accessed, the frequency of that page
            // number needs to be updated in the array that tracks how often each page is requested.
            // The page number itself is used as the array index, and what gets stored in the array
            // is the frequency.
            page_frequencies[pages[pg_index]] += 1; 
        }
        pg_index++;
    }
    printf("\nThe system had %d page faults during the process execution.\n", pageFault);
    printf("There were %d minor page faults during the process execution.\n", rw_counter);
}

/*
 * This function adds the page into the pages frame (table). It checks if the table 
 * is full, and if it isn't, then inserts the page.
 * Preconditions: The function must be called with the page number that is to be added.
 * Postcondtions: None.
 */
void insertPage(int pg_num) {
    int table_idx;
    // First we check if the page we are inserting into the table is already in
    // the table, because then we don't need to add it.
    if (inTable(pg_num)) {
        // Gets the index of the page number in the table.
        table_idx = table_index(pg_num);
        // Since the page is already in the page frame, then it is getting called again
        // to be modified by the system and we need to set the dirty bit to 1, which
        // is "true".
        table[table_idx].dirty_bit = 1;
    }
    else {
        // First we check if the frames are full.
        if (table_isFull()) {
            deletePage(); // Calls deletePage to first delete a page from the frame.
            insertPage(pg_num);  // Calls the function again to insert page into a newly empty frame.
        }
        // Then we check if there is already an empty slot in the table.
        else if (has_slot()) {
            // Gets the index of the empty spot.
            table_idx = empty_spot();
            // Adds the page at the specified index.
            table[table_idx].page_num = pg_num;
            // Increases the table counter.
            table_cnt++;
            // If we are adding a page into the page frame, then it has not been modified
            // before by the system and we need to set the dirty bit to 0, which is "false".
            table[table_idx].dirty_bit = 0;
        }
    }
    // Uncomment for debugging.
    // printf("Added to table -> %d\n", table[table_idx].page_num);
    // printf("The table = ");
    // display_table();
}

/*
 * This function deletes a page from the pages frame (table). First it checks if 
 * the table is empty, and if it's empty it prints an error. Otherwise it deletes 
 * the page at the specified table index position. It also checks if the page that
 * that is about to be deleted has the dirty bit set, and if so, increases
 * the read/write counter.
 * Preconditions: None
 * Postconditions: None
 */
void deletePage() {
    int lfu_pgnum, table_idx;
    // First checks if the table is empty.
    if (table_isEmpty()) {
        printf("Error! Table is empty. Unable to delete items.\n");
    }
    // Needs to find a page to replace by calling get_LFU().
    else {
        // Gets a page number for the page that is the least frequently used.
        lfu_pgnum = get_LFU();
        // Finds the index for where that page is currently located in the table.
        table_idx = table_index(lfu_pgnum);
        
        // Checks if the page has the dirty bit set and was modified.
        if (table[table_idx].dirty_bit == 1) {
            rw_counter++;  // Increments the read/write counter.
        }
        // Marks the page in the table at that index as empty.
        table[table_idx].page_num = -1;
        table_cnt--; // Decrease the table index counter.

        // Resets the frequency of the page number in the frequency array, because
        // the page was deleted.
        page_frequencies[lfu_pgnum] = 0;

        // Uncomment for debugging.
        // printf("Removed -> %d\n", table[table_idx].page_num);
        // display_table();
    }
}

/*
 * This function checks if the table is full.
 * Preconditions: None
 * Posconditions: The function returns true if the table is full, otherwise it returns false.
 */
bool table_isFull() {
    if (table_cnt == NFRAME) {
        return true;
    }
    return false;
}

/*
 * This function checks if the table is empty.
 * Preconditions: None
 * Posconditions: The function returns true if the table is empty, otherwise it returns false.
 */
bool table_isEmpty() {
    if (table_cnt == 0) {
        return true;
    }
    return false;
}

/*
 * The function checks if there is an empty spot in the table.
 * Preconditions: None
 * Postconditions: Returns true if there's an emptpy spot and false otherwise.
 */
bool has_slot() {
    for (int i = 0; i < NFRAME; i++) {
        if(table[i].page_num == -1) {
            return true;
        }
    }
    return false;
}

/*
 * The function checks if there is an empty spot in the table and returns the index
 * of that empty spot.
 * Preconditions: None
 * Postconditions: Returns the index of the empty spot. It returns -2 if for some
 *                 reason it does not find an empty spot.
 */
int empty_spot() {
    for (int i = 0; i < NFRAME; i++) {
        if(table[i].page_num == -1) {
            return i;
        }
    }
    return (-2);
}

/*
 * The function checks if the page is already in the table.
 * Preconditions: The function is called with the page number that needs to be searched.
 * Postconditions: The function returns true if the page is in the table, false otherwise.
 */
bool inTable(int pg_num) {
    for (int i = 0; i < NFRAME; i++) {
        if (table[i].page_num == pg_num) {
            return true;
        }
    }
    return false;
}

/*
 * The function gets the index of the page in the table by searching for the page number.
 * Preconditions: The function is called with the page number that needs to be searched.
 * Postconditions: The function returns the index of the page in the table.
 */
int table_index(int page_num) {
    for (int i = 0; i < NFRAME; i++) {
        if (table[i].page_num == page_num) {
            return i;
        }
    }
    return (-2);
}

/*
 * This function sorts through the page_frequencies array and the current entries
 * in the page table to find the smallest frequency and its corresponding page number.
 * Preconditions: None
 * Postconditions: The function returns the page number that should be replaced
 *                 because it has the smallest frequency.
 */
int get_LFU() {
    // Goes through the numbers currently in the page table to find their frequencies
    // to store in a separate array.
    int pg_freq_temp[11];
    // Prepopulates the temp array with large numbers so that the array does not contain 0.
    for (int k = 0; k < 11; k++) {
        pg_freq_temp[k] = 200;
    }
    int pg_num;
    for (int i = 0; i < NFRAME; i++) {
        // Gets the page number from the current table.
        pg_num = table[i].page_num;
        // Puts the page frequency from the frequencies array into the temp page frequency.
        // We only need to consider frequencies from the current table.
        pg_freq_temp[pg_num] = page_frequencies[pg_num];
    }
    // Finds the smallest number in the temp array.
    int final_pg = 0;
    int smallest = pg_freq_temp[0];
    for (int j = 1; j < 11; j++) {
        if (smallest > pg_freq_temp[j] || smallest == pg_freq_temp[j]) {
            smallest = pg_freq_temp[j];
            final_pg = j;  // This sets the page number to whatever page frequency is smaller.
        }
    }
    return final_pg;
}

/*
 * This function prints the table.
 * Preconditions: None
 * Postconditions: It prints the table to the console.
 */
void display_table() {
    if (table_isEmpty()) {
        printf("The table is empty.\n");
    }
    else {
        for (int i = 0; i < NFRAME; i++) {
            printf("%d\t", table[i].page_num);
        }
        printf("\n");
    }
}
