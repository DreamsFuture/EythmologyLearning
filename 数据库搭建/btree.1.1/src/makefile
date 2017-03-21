#
#	Copr 1991, 1992, 1995 Larry A. Walker & Co.
#

#PICK YOUR COMPILER and CFLAGS
#CC=cc -Aa
CC=gcc
#CC=CC
#CC=g++
CFLAGS=  -DSYSV -D_HPUX_SOURCE -g
OBJS= btree.o bfh.o utils.o debug.o dll.o semaphore.o
INC= -I../include
LIB= -L../lib

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $<

all : libindex.a btdebug

libindex.a : $(OBJS)
	ar rv ../lib/libindex.a $(OBJS) 

btdebug : btdebug.o libindex.a
	$(CC) -o btdebug btdebug.o $(LIB) -lindex $(INC)
	mv btdebug ../bin

clean : 
	rm -f *.o *.a dump core testbfh gen *.idx Crossref.* cscope.out \
	ungen list testfile btdebug *.log idxdump browse.list dump.out *.HOLD \
	dbtest* dlltest
