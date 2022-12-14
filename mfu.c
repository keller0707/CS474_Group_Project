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

/* Global Variables */
Page table[NFRAME];       // We create a table to simulate a 10 page frame structure.
int table_count = 0;      // Helps track if the table is empty or full.
int page_frequencies[11]; // Array to store the frequencies of each page.

/*
 * This function simulates how an MFU (Most Frquently Used) algorithm replaces pages in
 * the page frame.
 * Preconditions : project3.c should call this function.
 * Postconitions : Prints number of page faults and number of writes.
 */
void run_mfu(){
    /* Initializes values */
    for (int i = 0; i < NFRAME; i++){ // Loop through Page
        table[i].page_num = -1;       // Set value to -1 (Empty)
    }                                 // END for

    // Holds the index of the table.
    int table_idx;

    while (pg_index != page_refs){
        if(mfu_inTable(pages[pg_index]) == false){
            insert(pages[pg_index]);
            pageFault++;
        } else {
            table_idx = mfu_table_index(pages[pg_index]);
            table[table_idx].dirty_bit = 1;
        } // ifElse

        page_frequencies[pages[pg_index]] += 1;

        // Update Index
        pg_index++; 
    } // END while
    printf("\nThe system had %d page faults during the process execution.\n", pageFault);
    printf("There were %d read/write actions to disk during the process execution.\n", rw_counter);
} // END run_mfu

/*
 * Insert a page into the array
 * Preconditions  : The array is not full
 * Postconditions : A filled page with desired page number
 */
void insert(int pg_num){
    // Holds the index of the table.
    int table_idx;

    // Check if page number is in the table
    if(mfu_inTable(pg_num)){
    	// Get the talbe index
        table_idx = mfu_table_index(pg_num);
        
        // Set Dirty bit to true
        table[table_idx].dirty_bit = 1;
        return;
    } // End if

    // If the table is full
    if(table_count == NFRAME){
        // Delete a page
        delete();
        
        // Insert new page
        insert(pg_num);
        return;
    } // END ifFull

    // Find empty spot
    table_idx = mfu_empty_spot();
    
    // Set new Page Number
    table[table_idx].page_num = pg_num;
    
    // Update table count
    table_count++;
    
    // Set bit
    table[table_idx].dirty_bit = 0;
    
    //printf("Added to table -> %d\n", table[table_idx].page_num);
    //printf("The table = ");
    //mfu_display_table();

} // END insertPage

/*
 * Clear a spot in the array
 * Preconditions  : The array exist 
 * Postconditions : A openned page
 */
void delete(){
    int mfu_pgnum, table_idx;
    
    // Check if the table is empty
    if (mfu_isEmpty()){
    	// Print Error.
        printf("ERROR: Table is empty");
        return;
    } // END if
    
    // Get Page index
    mfu_pgnum = get_mfu();
    
    // Set table index
    table_idx = mfu_table_index(mfu_pgnum);
    
    // Check if its a dirty bit
    if (table[table_idx].dirty_bit == 1) rw_counter++;

    // Remove page number
    table[table_idx].page_num = -1;
    
    // Update Count
    table_count--;
    
    // Update Freq.
    page_frequencies[mfu_pgnum] = 0;
      // Uncomment for debugging.
    //printf("Removed -> %d\n", table[table_idx].page_num);
    //mfu_display_table();

} // END delete

/*
 * Checks to see if table is empty
 * Preconditions  : Table exists
 * Postconditions : If its empty or not
 */
bool mfu_isEmpty() {
    // Check if count is 0
    if (table_count == 0) return true;

    // Return false
    return false;
} // END mfu_isEmpty

/*
 * Checks if given page number is in the table
 * Preconditions  : The array is not full
 * Postconditions : if its in (true) or not (false)
 */
bool mfu_inTable(int pg_num) {
    for (int i = 0; i < NFRAME; i++) {                // Loop through NFRAME
        if (table[i].page_num == pg_num) return true; //  Check if element holds page number
    }                                                 // END for
    // Return false
    return false;
} // END mfu_inTable

/*
 * Finds table with given page number
 * Preconditions  : The array is not full
 * Postconditions : index of table
 */
int mfu_table_index(int page_num) {
    for (int i = 0; i < NFRAME; i++) {               // Loop through NFRAME
        if (table[i].page_num == page_num) return i; // Check if element holds page number
    }                                                // END for

    // Return -2 (not found)
    return (-2);
} // END mfu_table_index

/*
 * Finds an empty spot within the array
 * Preconditions  : The array is not full
 * Postconditions : index of a open spot
 */
int mfu_empty_spot() {
    
    for (int i = 0; i < NFRAME; i++) {        // Loop through NFRAME
        if(table[i].page_num == -1) return i; // Check if empty page is found
    }                                         // END for

    // Return -2 (not found)
    return (-2);
} // END mfu_empty_spot

/*
 * Display the table within the NFRAME limit
 * Precondition  : Table is not empty
 * Postcondition : Print elements in the tables.
 */
void mfu_display_table() {
    
    if (mfu_isEmpty()) {                 // Check if table empty 
        printf("The table is empty.\n"); // Print error
        return;                          // Exit funtion
    }                                    // END if
    
    for (int i = 0; i < NFRAME; i++) {                 // Loop through Frame
        printf("Page Num: %d\t", table[i].page_num);  // Print table page num
        printf("Freq Num: %d\t", page_frequencies[i]); // Print freq. table
    }                                                  // END for

        // Print new lines
        printf("\n");
} // END mfu_display_table

/*
 * This function returns the page index of the 
 * Most frequently used page.
 * Precondition  : That the freq. table is not empty
 * Postcondition : The index of the most freq. used
 */
int get_mfu(){
    // Set inital values
    int pg = 0;
    int most = page_frequencies[0];
    
    // Loop through Freq. array
    for (int i = 0; i < 11; i++){
        if(most < page_frequencies[i]){ // If found a higher number, update
            most = page_frequencies[i]; // Update most
            pg = i;                     // Update page
        }                               // END if
    }                                   // END for
    
    // Return index of page_freq.
    return pg;
} //  END get_mfu
