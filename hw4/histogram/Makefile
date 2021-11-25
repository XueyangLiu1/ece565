#----- Modify CC and CFLAGS as appropriate in your environment
CC = gcc
CFLAGS = -O3 -fopenmp

LIB = -lm -lc

UNITFILES = 

#------------ EXECUTABLE FILES ---------------

histo : histo.o histo_locks.o histo_atomic.o histo_creative.o $(UNITFILES)
	$(CC) $(CFLAGS) -o histo histo.o $(UNITFILES) $(LIB)
	$(CC) $(CFLAGS) -o histo_locks histo_locks.o $(UNITFILES) $(LTB)
	$(CC) $(CFLAGS) -o histo_atomic histo_atomic.o $(UNITFILES) $(LTB)
	$(CC) $(CFLAGS) -o histo_creative histo_creative.o $(UNITFILES) $(LTB)
	
# histo_locks: histo_locks.o $(UNITFILES)
# 	$(CC) $(CFLAGS) -o histo_locks histo_locks.o $(UNITFILES) $(LTB)

# histo_atomic: histo_atomic.o $(UNITFILES)
# 	$(CC) $(CFLAGS) -o histo_atomic histo_atomic.o $(UNITFILES) $(LTB)

# histo_creative: histo_creative.o $(UNITFILES)
# 	$(CC) $(CFLAGS) -o histo_creative histo_creative.o $(UNITFILES) $(LTB)

clean:
	rm -f histo histo_locks histo_creative histo_atomic *.o

histo.o : histo.c
	$(CC) $(CFLAGS) -c histo.c $(INCLUDE) 

histo_locks.o : histo_locks.c
	$(CC) $(CFLAGS) -c histo_locks.c $(INCLUDE)

histo_atomic.o : histo_atomic.c
	$(CC) $(CFLAGS) -c histo_atomic.c $(INCLUDE)

histo_creative.o : histo_creative.c
	$(CC) $(CFLAGS) -c histo_creative.c $(INCLUDE)


