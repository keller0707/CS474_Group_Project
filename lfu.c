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
int table_cnt = 0;

/*
 * This function 
 * 
 * 
 * 
 * Preconditions: The 
 *                b
 * Postconditions: The function 
 *                 the
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
            // Adds the page to the page table.
            insertPage(pages[pg_index]);
            // Because it's not in the table, it's a page fault so we increment the fault counter.
            pageFault++;
        }
        else {
            // Page is already in the table and we need to find the page to get its index.
            int table_idx = table_index(pages[pg_index]);
            // Uses the index to set the dirty bit to 1 (true) because memory is accessing 
            // the page again to modify it. 
            table[table_idx].dirty_bit = 1;
        }
        pg_index++;
    }
    printf("\nThe system had %d page faults during the process execution.\n", pageFault);
    printf("There were %d read/write actions to disk during the process execution.\n", rw_counter);
}

/*
 * This function adds the page into the pages frame (table). It checks if the table 
 * is full, and if it isn't, then inserts the page.
 * Preconditions: The function must be called with the page number that is to be added.
 * Postcondtions: None.
 */
void insertPage(int pg_num) {
    int table_idx;
    // First we check if the frames are full.
    if (table_isFull()) {
        // Get an index for the page that is the least frequently used.
        table_idx = get_LFU();
        deletePage(table_idx); // Calls deletePage to first delete a page from the frame.
        insertPage(pg_num);  // Add the page to the newly open frame.
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
        // by the system and we need to set the dirty bit to 0, which is "false".
        table[table_idx].dirty_bit = 0;
    }
    else {
        // There is no empty spot and we have to find the least frequently used page.
        table_idx = get_LFU();
        // Adds the page at the specified index.
        table[table_idx].page_num = pg_num;
        // Increases the table counter.
        table_cnt++;
        // If we are adding a page into the page frame, then it has not been modified
        // by the system and we need to set the dirty bit to 0, which is "false".
        table[table_idx].dirty_bit = 0;
    }
    // Uncomment for debugging.
    printf("Added to table -> %d\n", table[table_idx].page_num);
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
void deletePage(int table_idx) {
    int pg; // Uncomment for debugging.
    // First checks if the table is empty.
    if (table_isEmpty()) {
        printf("Error! Table is empty. Unable to delete items.\n");
    }
    else {
        // Checks if the page has the dirty bit set and was modified.
        if (table[table_idx].dirty_bit == 1) {
            rw_counter++;  // Increments the read/write counter.
        }
        pg = table[table_idx].page_num;  // Uncomment for debugging.
        // Marks the table at that index as empty.
        table[table_idx].page_num = -1;
        table_cnt--; // Decrease the table index counter.

        // Uncomment for debugging.
        printf("Removed -> %d\n", pg);
        display_table();
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
bool inTable(int page_num) {
    for (int i = 0; i < NFRAME; i++) {
        if (table[i].page_num == page_num) {
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
 * This function
 * Preconditions:
 * Postconditions:
 */
int get_LFU() {
    /// To be developed.
    return 2;
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
