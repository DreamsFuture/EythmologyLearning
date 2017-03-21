#
#	Copr 1991, 1992, 1995 Larry A. Walker & Co.
#

all : 
	cd src; make
	cd tests; make

clean :
	rm -f lib/*
	rm -f bin/*
	cd src; make clean
	cd tests; make clean


dist : 
	rm btree.t*
	make clean
	sleep 2
	tar cvf btree.tar *
	gzip btree.tar
	
