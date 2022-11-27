#-----------------------------------------------------------------------------
#  Group: Nasley Chumacero-Martin, Zachariah Cleveland, and Keller Sedillo
#  CS 474 - Operating Systems
#  Fall 2022
#  Project 3: Measuring the Performance of Page Replacement Algorithms

#  This makefile runs the main target which builds the program and
#  creates an executable. It also contains a target to create a clean
#  build by deleting the previous executable, if it exists.
#-----------------------------------------------------------------------------

# This main target runs both the compile functions and the clean functions
# with the commands "make" or "make all".
all:	clean proj3

# This program checks that the files exist before compiling. 
# If the source files exist, then it compiles them.
proj3:	project3.c readfile.c fifo.c lfu.c lru.c functions.h 
	gcc -Wall project3.c readfile.c fifo.c lfu.c lru.c -o pageSimulator

# The clean target deletes any prior objects from other compilations so that the program can 
# be built cleanly every time the make command runs.
clean:
	rm -f pageSimulator

# The program contains an additional target to run the program easily with the command "make run".
# It just ensures that the exectuable exists before running.
run:	pageSimulator
	./pageSimulator