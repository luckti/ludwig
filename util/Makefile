###############################################################################
#
#  Makefile
#
#  Various targets for utility programs to perform pre-processing
#  and post-processing tasks.
#
#  The Makefile does not have the dependencies encoded completely,
#  so you will need to ensure the correct serial targetDP and ludwig
#  libraries are available manually before action in this directory.
#  Intended to be run in serial.
#
#  Edinburgh Soft Matter and Statistical Physics Group and
#  Edinburgh Parallel Computing Centre
#
#  (c) 2016 The University of Edinburgh
#  Contributing authors:
#  Kevin Stratford (kevin@epcc.ed.ac.uk)
#
###############################################################################

include ../Makefile.mk

LIBS = ../mpi_s/libmpi.a ../src/libludwig.a -L../targetDP -ltarget -lm
INC = -I. -I../src -I../targetDP -I../mpi_s

colloid_init:	colloid_init.c
	$(CC) $(CFLAGS) $(INC) colloid_init.c $(LIBS) -o colloid_init

extract_colloids:	extract_colloids.c
	$(CC) $(CFLAGS) $(INC) extract_colloids.c $(LIBS) -o extract_colloids

capillary:	capillary.c
	$(CC) $(CFLAGS) capillary.c -o capillary

.PHONY : clean
clean:
	rm -f colloid_init extract_colloids capillary

