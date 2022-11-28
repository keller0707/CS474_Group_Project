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

int array[NFRAMES]; // We create a queue to simulate a 10 page frame structure.
int size=0; //Size of array.
int LRU_pageFault = 0;     // This counter will keep track of how many page faults occur
int lpg_index = 0;   // This variable will control the index of the page numbers array.

/*
 * This function uses the page numbers in the pages array to load up Array that
 * represents the page frames.
 *
 * Preconditions: The function to read the memory file has to be called before calling
 *                this function, because the array pages[] has to be filled.
 * Postconditions:
 */
void run_lru() {
    // Uncomment for debugging:
    //printf("page numbers:\n");
    //for (int i = 0; i < page_refs; i++) {
    //	printf("%d, ", pages[i]);
    //}
    //printf("\n");
    
    // Loop runs while index hasn't reached the size of the pages array.
    while (lpg_index != page_refs) {
        /// Uncomment for debugging:
    	//printf("page#: %d\n", pages[lpg_index]);
        
        
        // Check if the page is not in the page frame yet
        if (inArray(pages[lpg_index]) == 0) {
            // This is a page fault so we increment the fault counter.
            LRU_pageFault++;
            
            // Enqueue the page and add it to a frame.
            addToArray(pages[lpg_index]);
        }
        
        else {
        	//No page fault.
        }
        
        // Uncomment for debugging:
        //printf("Array:");
        //ArrayDisplay();
        
        //increase page index.
        lpg_index++;
    }
    printf("\nThe system had %d page faults during the process execution.\n", LRU_pageFault);

}

/* addToArray()
* adds the given value to the array. If new and out, replace oldest unused value.
* preconditions: called on by run_lrc() only
* postconditions: 
*/
void addToArray(int new) {
	
	//if Array is full, replace one of the values.
	if (size == NFRAMES) {
		replace(last_recently_used(), new);
	}
	
	//if Array isn't full, replace the next value.
	else {
		array[size] = new;
		size++;
	}
}

/* replace()
* replaces old page number with current page number.
* preconditions: called on by addToArray() only. Old value must be in Array.
*/
void replace(int old, int new) {
	for (int i=0; i < NFRAMES; i++) {
		if (array[i] == old) {
			array[i] = new;
			return;
		}
	}
	printf("ERROR! Old page to replace could not be found in replace()");

}

/* last_recently_used()
 * This function fetches the value of the last recently used variable.
 * preconditions: relies on lpg_pages and lpg_index.
 */
int last_recently_used() {
	//min will be the page with the last accessed page.
	int lru[NFRAMES];
	int count = 0; //must count up to NFRAMES
	int found = false;
	
	//go to first value
	for (int i=lpg_index-1; count < NFRAMES; i--) {
	
		//check if pages[i] is in lru
		for (int j=0; j < count; j++) {
			if (lru[j] == pages[i]) {
				found = true;
				break;
			}
		}
		//if pages[i] not in lru, add it to lru
		if (found == true) {
			found = false;
		}
		else {
			lru[count] = pages[i];
			count++;
		}
	}
	// Uncomment to debug:
	// printf("last used: %d\n", lru[count-1]);
	
	//lru[FRAMES-1] is the last accessed page.
	return lru[count-1];		
}



/*
 * This function prints the queue.
 * Preconditions: None
 * Postconditions: It prints the queue to the console.
 */
void ArrayDisplay() {
    if (size==0) {
        printf("The array is empty.\n");
    }
    else {
        for (int i = 0; i < size; i++) {
            printf("%d\t", array[i]);
        }
        printf("\n");
    }
}


int inArray(int var) {
	for (int i=0; i < NFRAMES; i++) {
		if (var == array[i]) return 1;
	} 
	return 0;
}
