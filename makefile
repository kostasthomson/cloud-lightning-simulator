#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = mpicxx
IDIR=include
ODIR=obj
SDIR=src
BDIR=bin

_OBJ=cl_sim.o task.o resource.o comm.o inputs.o cell.o power.o tce.o netw.o stat.o gs.o brokers.o misc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEPS = task.h resource.h cell.h inputs.h comm.h power.h tce.h netw.h stat.h gs.h brokers.h misc.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CFLAGS  = -O3 -std=c++11 -lm -fopenmp -I$(IDIR)

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: all

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
all:  $(OBJ)
	$(CC) $(CFLAGS) -o $(BDIR)/cl_sim $(OBJ)

# To create the object file countwords.o, we need the source
# files countwords.c, scanner.h, and counter.h:
#
$(ODIR)/%.o:  $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

#$(ODIR)/cl_sim.o:  $(SDIR)/cl_sim.cpp $(DEPS) 
#	$(CC) $(CFLAGS) -c $(SDIR)/cl_sim.cpp -o $@

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) all $(ODIR)/*.o $(ODIR)/*~ $(BDIR)/cl_sim
