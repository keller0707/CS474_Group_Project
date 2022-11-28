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

extern int page_refs;


Page table[NFRAME]; // We create a table to simulate a 10 page frame structure.
int table_count = 0;  // Helps track if the table is empty or full.

int page_frequencies[11]; // Array to store the frequencies of each page.

void run_mfu(){
    /* Initializes values */
    for (int i = 0; i < NFRAME; i++){ // Loop through Page
        table[i].page_num = -1;       // Set value to -1 (Empty)
    }                                 // END for

    // Holds the index of the table.
    int table_idx;

    while (pg_index != page_refs){
        if(inTable(pages[pg_index]) == false){
            insert(pages[pg_index]);
            pageFault++;
        } else {
            table_idx = table_index(pages[pg_index]);
            table[table_idx].dirty_bit = 1;
        } // ifElse

        page_frequencies[pages[pg_index]] += 1;

        // Update Index
        pg_index++; 
    } // END while
} // END run_mfu

void insert(int pg_num){
    // Holds the index of the table.
    int table_idx;

    if(inTable(pg_num)){
        table_idx = table_index(pg_num);
        table[table_idx].dirty_bit = 1;
        return;
    } // End if

    if(table_count == NFRAME){
        delete();
        insert(pg_num);
        return;
    } // END ifFull

    table_idx = empty_spot();
    table[table_idx].page_num = pg_num;
    table_count++;
    table[table_idx].dirty_bit = 0;

} // END insertPage

void delete(){
    int mfu_pgnum, table_idx;
    if (mfu_isEmpty()){
        printf("ERROR: Table is empty");
        return;
    } // END if
    
    mfu_pgnum = get_mfu();
    table_idx = table_index(mfu_pgnum);
    if (table[table_idx].dirty_bit == 1) rw_counter++;

    table[table_idx].page_num = -1;
    table_count--;
    page_frequencies[mfu_pgnum] = 0;

} // END delete

bool mfu_isEmpty() {
    if (table_count == 0) {
        return true;
    }
    return false;
}

int get_mfu(){
    int pg = 0;
    int most = page_frequencies[0];
    for (int i = 0; i < sizeof(page_frequencies); i++){
        if(most < page_frequencies[i]){
            most = page_frequencies[i];
            pg = i;
        } // END if
    } // END for
    return pg;
} //  END get_mfu
