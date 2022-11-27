#include <stdio.h>
#include "functions.h"

/* -----------------------------------------------------------------------------------
 * Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
 * CS 474 - Operating Systems
 * Fall 2022
 * Project 3: Measuring the Performance of Page Replacement Algorithms
 * ----------------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------------------------
 * The program 
 * 
 * Input: The program 
 * Output: The program
 * -------------------------------------------------------------------------------------------- */

int main() {

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
         break;
      case 3:
         printf("You chose LFU.\n");
         break;
      case 4:
         printf("You chose MFU.\n");
         break;
      case 5:
         printf("You chose OPT.\n");
         break;
   }

   return 0;  // Ends main.
}