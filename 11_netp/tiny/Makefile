CC = gcc
CFLAGS = -O2 -Wall -I .

# This flag includes the Pthreads library on a Linux box.
# Others systems will probably require something different.
LIB = -lpthread

all: tiny cgi

tiny: tiny.c csapp.o
	$(CC) $(CFLAGS) -o tiny tiny.c csapp.o $(LIB)

# for testing of chapter 11 hw
tiny11: ../hw/p.tiny.c csapp.o
	$(CC) $(CFLAGS) -o tiny ../hw/p.tiny.c  csapp.o $(LIB)

# for testing of chapter 12 how
# need to copy .c file to ./tiny12.c before compile
tiny12: tiny12.c sbuf.c csapp.o
	$(CC) $(CFLAGS) -o tiny tiny12.c sbuf.c csapp.o $(LIB)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

cgi:
	(cd cgi-bin; make)

clean:
	rm -f *.o tiny *~
	(cd cgi-bin; make clean)

