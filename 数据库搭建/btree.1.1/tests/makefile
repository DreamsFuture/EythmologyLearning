#
#	Copr 1991, 1992, 1995 Larry A. Walker & Co.
#

#PICK YOUR COMPILER and CFLAGS
#CC=cc -Aa
CC=gcc
#CC=CC
#CC=g++
CFLAGS=  -DSYSV -D_HPUX_SOURCE -g
LIB= -L../lib
INC= -I../include

all : testbfh gen list ungen idxdump dlltest

testbfh : testbfh.c
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@ $@.c -lindex
	mv $@ ../bin

gen : gen.c
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@  $@.c -lindex
	mv $@ ../bin

list : list.c
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@ $@.c -lindex
	mv $@ ../bin

ungen : ungen.c
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@ $@.c -lindex
	mv $@ ../bin

idxdump : idxdump.c
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@ $@.c -lindex
	mv $@ ../bin

dlltest : dlltest.c 
	$(CC) $(LIB) $(INC) $(CFLAGS) -o $@ $@.c -lindex
	mv $@ ../bin

clean : 
	rm -f *.o *.a dump core testbfh gen *.idx Crossref.* cscope.out \
	ungen list testfile btdebug *.log idxdump browse.list dump.out *.HOLD \
	dbtest* dlltest
