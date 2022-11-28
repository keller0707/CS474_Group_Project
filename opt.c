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

int opt_array[NFRAMES]; // We create a queue to simulate a 10 page frame structure.
int opt_size=0; //Size of array.
int opt_pageFault = 0;     // This counter will keep track of how many page faults occur
int opt_index = 0;   // This variable will control the index of the page numbers array.

/*
 * This function uses the page numbers in the pages array to load up Array that
 * represents the page frames.
 *
 * Preconditions: The function to read the memory file has to be called before calling
 *                this function, because the array pages[] has to be filled.
 * Postconditions:
 */
void run_opt() {
    // Uncomment for debugging:
    printf("page numbers:\n");
    for (int i = 0; i < page_refs; i++) {
    	printf("%d, ", pages[i]);
    }
    printf("\n");
    
    // Loop runs while index hasn't reached the size of the pages array.
    while (opt_index != page_refs) {
        /// Uncomment for debugging:
    	//printf("page#: %d\n", pages[opt_index]);
        
        
        // Check if the page is not in the page frame yet
        if (opt_inArray(pages[opt_index]) == 0) {
            // This is a page fault so we increment the fault counter.
            opt_pageFault++;
            
            // Enqueue the page and add it to a frame.
            opt_addToArray(pages[opt_index]);
        }
        
        else {
        	//No page fault.
        }
        
        // Uncomment for debugging:
        printf("Array:\t");
        opt_ArrayDisplay();
        
        //increase page index.
        opt_index++;
    }
    printf("\nThe system had %d page faults during the process execution.\n", opt_pageFault);

}

/* addToArray()
* adds the given value to the array. If new and out, replace oldest unused value.
* preconditions: called on by run_lrc() only
* postconditions: 
*/
void opt_addToArray(int new) {
	
	//if Array is full, replace one of the values.
	if (opt_size == NFRAMES) {
		opt_replace(last_to_be_used(), new);
	}
	
	//if Array isn't full, replace the next value.
	else {
		opt_array[opt_size] = new;
		opt_size++;
	}
}

/* replace()
* replaces old page number with current page number.
* preconditions: called on by addToArray() only. Old value must be in Array.
*/
void opt_replace(int old, int new) {
	for (int i=0; i < NFRAMES; i++) {
		if (opt_array[i] == old) {
			opt_array[i] = new;
			return;
		}
	}
	printf("ERROR! Old page to replace could not be found in replace()");

}

/* last_recently_used()
 * This function fetches the value of the last recently used variable.
 * preconditions: relies on opt_pages and opt_index.
 */
int last_to_be_used() {
	
	//go to last value to be referenced in pages
	for (int i=page_refs ; i > 0; i--) {
	
		//check if pages[i] is in array
		if (opt_inArray(pages[i]) == 1) return pages[i];
		//if pages[i] not in lru, throw error.
		
		
	}
	
	printf("Array value not found in pages!!");
	return -1;
	
}


/*
 * This function prints the queue.
 * Preconditions: None
 * Postconditions: It prints the queue to the console.
 */
void opt_ArrayDisplay() {
    if (opt_size==0) {
        printf("The array is empty.\n");
    }
    else {
        for (int i = 0; i < opt_size; i++) {
            printf("%d\t", opt_array[i]);
        }
        printf("\n");
    }
}


int opt_inArray(int var) {
	for (int i=0; i < NFRAMES; i++) {
		if (var == opt_array[i]) return 1;
	} 
	return 0;
}
