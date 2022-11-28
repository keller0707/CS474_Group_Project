#include <stdio.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------------------
 * The program simulates a the action of a virtual memory system with a single level page
 * table and how it handles page requests with specific page replacement algorithms. It asks
 * or user input to run the 5 page replacement algorithms. It runs FIFO, LRU, LFU
 * MFU, and OPT. Each page replacement algorithm is run by the progam by calling a specific
 * function. For example, to run FIFO, the program calls "run_fifo()". Each algorithm is run
 * with a reference string from "memory_data.txt", which simulates the page requests from 
 * a process.
 * Input: The program asks for user input to run the selected algorithm.
 * Output: The program runs each page replacement algorithm, prints selected output such as
 *         the page faults that occur with each algorithm, and then exits.
 * -------------------------------------------------------------------------------------------- */

int main() {
   pageFault = 0;     // This counter will keep track of how many page faults occur.
   pg_index = 0;   // This variable will control the index of the page numbers array.
   rw_counter = 0;  // The read/write counter.

   // The program reads in the "memory trace" from a file. The memory file contains
   // a string of numbers which represent the memory address being read.
   read_file();

   int choice;
   // Asks the user for input to run a page-replacement algorithm.
   printf("\n** Enter a number for the page replacement algorithm to run. **\n");
   printf("1. FIFO (First-In First-Out)\n");
   printf("2. LRU (Least Recently Used)\n");
   printf("3. LFU (Least Frequently Used)\n");
   printf("4. MFU (Most Frequently Used)\n");
   printf("5. OPT (Optimal Page Replacement)\n\n");
   scanf("%d", &choice);

   switch(choice) {
      case 1:
         printf("You chose FIFO.\n");
         run_fifo();
         break;
      case 2:
         printf("You chose LRU.\n");
         run_lru();
         break;
      case 3:
         printf("You chose LFU.\n");
         run_lfu();
         break;
      case 4:
         printf("You chose MFU.\n");
         run_mfu();
         break;
      case 5:
         printf("You chose OPT.\n");
         run_opt();
         break;
   }

   return 0;  // Ends main.
}
