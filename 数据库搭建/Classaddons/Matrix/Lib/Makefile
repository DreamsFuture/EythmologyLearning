CC = g++
INCLUDES = -I../Include -I../.. -I../../Misc
DEFINES = -DUSE_SET -DOLD #-DMDEBUG 
CFLAGS = -g $(INCLUDES) -O4 -Wall $(DEFINES)
ODIR = obj
OBJS=SparseMatrix.o DenseMatrix.o IdentityMatrix.o Entry.o \
	Tridiagonal.o Solver.o Vector.o Banded.o Jacobi.o Gauss.o \
	Conj.o ConjJacob.o SOR.o BIMatrix.o Matrix.o SVector.o \
	# YSMP.o dysmp.o
NOBJS := $(foreach obj,$(OBJS),nobj/$(obj))
POBJS := $(foreach obj,$(OBJS),pobj/$(obj))

all: libMatrix++.a libMatrix_p++.a

libMatrix++.a: $(NOBJS)
	ar rv libMatrix++.a  $(NOBJS)
	ranlib libMatrix++.a

libMatrix_p++.a: $(POBJS)
	ar rv libMatrix_p++.a  $(POBJS)
	ranlib libMatrix_p++.a

nobj/%.o:	%.c
	@if [ ! -d nobj ]; then mkdir nobj; fi
	$(CC) -c $(CFLAGS) $< -o $@

pobj/%.o:	%.c
	@if [ ! -d pobj ]; then mkdir pobj; fi
	$(CC) -c -pg $(CFLAGS) $< -o $@

depend:
	touch tmp
	makedepend -ppobj/ $(DEFINES) $(INCLUDES) -I$(CPPINC) *.[c]
	makedepend -pnobj/ $(DEFINES) $(INCLUDES) -I$(CPPINC) *.[c] -f tmp
	cp Makefile Makefile.tmp
	cat Makefile.tmp tmp > Makefile
	rm Makefile.tmp tmp tmp.bak

clean:
	-rm $(NOBJS) $(POBJS)
	-rm libMatrix++.a
	-rm libMatrix_p++.a

# DO NOT DELETE THIS LINE -- make depend depends on it.

pobj/BIMatrix.o: ../Include/BIMatrix.h ../Include/Matrix.h
pobj/BIMatrix.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/BIMatrix.o: ../Include/Vector.h ../../Misc/GC.h
pobj/BIMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/BIMatrix.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h
pobj/BIMatrix.o: ../../Set/Set.h /usr/include/CC/math.h ../Include/Entry.h
pobj/BIMatrix.o: ../Include/SVector.h ../../Set/AssocSet.h
pobj/Banded.o: ../Include/SparseMatrix.h ../Include/Matrix.h
pobj/Banded.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/Banded.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
pobj/Banded.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
pobj/Banded.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
pobj/Banded.o: ../Include/Entry.h ../Include/Banded.h ../Include/Solver.h
pobj/Banded.o: ../../Misc/Timer.h /usr/include/CC/sys/time.h
pobj/Banded.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
pobj/Banded.o: /usr/include/CC/time.h /usr/include/prof.h
pobj/Conj.o: ../Include/Conj.h ../Include/Solver.h ../../Set/SimpleSet.h
pobj/Conj.o: ../../Set/Set.h /usr/include/CC/iostream.h
pobj/Conj.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/Conj.o: ../Include/Entry.h /usr/include/CC/stdio.h ../../Misc/GC.h
pobj/Conj.o: /usr/include/CC/stddef.h ../Include/Vector.h
pobj/Conj.o: /usr/include/CC/math.h ../../Misc/Timer.h
pobj/Conj.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/Conj.o: /usr/include/CC/unistd.h ../Include/Matrix.h
pobj/ConjJacob.o: /usr/include/CC/new.h /usr/include/CC/stddef.h
pobj/ConjJacob.o: /usr/include/CC/std.h /usr/include/CC/_G_config.h
pobj/ConjJacob.o: /usr/include/CC/stdlib.h /usr/include/CC/string.h
pobj/ConjJacob.o: /usr/include/CC/memory.h /usr/include/CC/unistd.h
pobj/ConjJacob.o: /usr/include/CC/stdio.h /usr/include/CC/errno.h
pobj/ConjJacob.o: /usr/include/CC/fcntl.h ../Include/ConjJacob.h
pobj/ConjJacob.o: ../Include/Solver.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/ConjJacob.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/ConjJacob.o: ../Include/Entry.h ../../Misc/GC.h ../Include/Vector.h
pobj/ConjJacob.o: /usr/include/CC/math.h ../../Misc/Timer.h
pobj/ConjJacob.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/ConjJacob.o: ../Include/Matrix.h ../Include/SparseMatrix.h
pobj/DenseMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/DenseMatrix.o: /usr/include/CC/stddef.h ../Include/DenseMatrix.h
pobj/DenseMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
pobj/DenseMatrix.o: ../Include/Vector.h ../../Misc/GC.h
pobj/DenseMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/DenseMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
pobj/DenseMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
pobj/Entry.o: ../Include/Entry.h /usr/include/CC/stdio.h
pobj/Entry.o: /usr/include/CC/_G_config.h ../../Misc/GC.h
pobj/Entry.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Entry.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/Gauss.o: ../Include/Gauss.h ../Include/Matrix.h /usr/include/CC/stdio.h
pobj/Gauss.o: /usr/include/CC/_G_config.h ../Include/Vector.h ../../Misc/GC.h
pobj/Gauss.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Gauss.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/Gauss.o: /usr/include/CC/math.h ../Include/Entry.h
pobj/Gauss.o: ../Include/SparseMatrix.h ../Include/Solver.h
pobj/Gauss.o: ../../Misc/Timer.h /usr/include/CC/sys/time.h
pobj/Gauss.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
pobj/IdentityMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/IdentityMatrix.o: /usr/include/CC/stddef.h ../Include/IdentityMatrix.h
pobj/IdentityMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
pobj/IdentityMatrix.o: ../Include/Vector.h ../../Misc/GC.h
pobj/IdentityMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/IdentityMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
pobj/IdentityMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
pobj/Jacobi.o: ../Include/Jacobi.h ../Include/Matrix.h
pobj/Jacobi.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/Jacobi.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
pobj/Jacobi.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
pobj/Jacobi.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
pobj/Jacobi.o: ../Include/Entry.h ../Include/SparseMatrix.h
pobj/Jacobi.o: ../Include/Solver.h ../../Misc/Timer.h
pobj/Jacobi.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/Jacobi.o: /usr/include/CC/unistd.h
pobj/Matrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/Matrix.o: /usr/include/CC/stddef.h ../Include/Matrix.h
pobj/Matrix.o: /usr/include/CC/stdio.h ../Include/Vector.h ../../Misc/GC.h
pobj/Matrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Matrix.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
pobj/Matrix.o: ../Include/Entry.h
pobj/SOR.o: ../Include/SOR.h ../Include/Matrix.h /usr/include/CC/stdio.h
pobj/SOR.o: /usr/include/CC/_G_config.h ../Include/Vector.h ../../Misc/GC.h
pobj/SOR.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/SOR.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/SOR.o: /usr/include/CC/math.h ../Include/Entry.h
pobj/SOR.o: ../Include/SparseMatrix.h ../Include/Solver.h ../../Misc/Timer.h
pobj/SOR.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/SOR.o: /usr/include/CC/unistd.h
pobj/SVector.o: ../Include/SVector.h ../../Set/AssocSet.h
pobj/SVector.o: ../../Set/SimpleSet.h ../../Set/Set.h
pobj/SVector.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/SVector.o: /usr/include/CC/_G_config.h ../Include/Vector.h
pobj/SVector.o: ../../Misc/GC.h /usr/include/CC/stddef.h
pobj/SVector.o: /usr/include/CC/math.h
pobj/Solver.o: ../Include/Solver.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/Solver.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Solver.o: /usr/include/CC/_G_config.h ../Include/Entry.h
pobj/Solver.o: /usr/include/CC/stdio.h ../../Misc/GC.h
pobj/Solver.o: /usr/include/CC/stddef.h ../Include/Vector.h
pobj/Solver.o: /usr/include/CC/math.h ../../Misc/Timer.h
pobj/Solver.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/Solver.o: /usr/include/CC/unistd.h
pobj/SparseMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/SparseMatrix.o: /usr/include/CC/stddef.h ../Include/SparseMatrix.h
pobj/SparseMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
pobj/SparseMatrix.o: ../Include/Vector.h ../../Misc/GC.h
pobj/SparseMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/SparseMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
pobj/SparseMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
pobj/SparseMatrix.o: ../Include/YSMP.h ../Include/Solver.h ../../Misc/Timer.h
pobj/SparseMatrix.o: /usr/include/CC/sys/time.h
pobj/SparseMatrix.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
pobj/SparseMatrix.o: ../Include/Jacobi.h ../Include/Banded.h
pobj/SparseMatrix.o: ../Include/Gauss.h ../Include/SOR.h
pobj/SparseMatrix.o: ../Include/ConjJacob.h ../Include/Conj.h
pobj/SparseMatrix.o: /usr/include/CC/ctype.h alleigen.h
pobj/Tridiagonal.o: ../Include/Tridiagonal.h ../Include/Matrix.h
pobj/Tridiagonal.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/Tridiagonal.o: ../Include/Vector.h ../../Misc/GC.h
pobj/Tridiagonal.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Tridiagonal.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h
pobj/Tridiagonal.o: ../../Set/Set.h /usr/include/CC/math.h ../Include/Entry.h
pobj/Vector.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
pobj/Vector.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/Vector.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
pobj/Vector.o: /usr/include/CC/math.h /usr/include/CC/assert.h
pobj/Vector.o: /usr/include/CC/stdio.h /usr/include/CC/ctype.h
pobj/YSMP.o: ../Include/YSMP.h /usr/include/CC/stdlib.h
pobj/YSMP.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
pobj/YSMP.o: ../Include/SparseMatrix.h ../Include/Matrix.h
pobj/YSMP.o: /usr/include/CC/stdio.h ../Include/Vector.h ../../Misc/GC.h
pobj/YSMP.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/YSMP.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
pobj/YSMP.o: ../Include/Entry.h ../Include/Solver.h ../../Misc/Timer.h
pobj/YSMP.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/YSMP.o: /usr/include/CC/unistd.h
pobj/dysmp.o: /usr/include/f2c.h
# DO NOT DELETE THIS LINE -- make depend depends on it.

nobj/BIMatrix.o: ../Include/BIMatrix.h ../Include/Matrix.h
nobj/BIMatrix.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/BIMatrix.o: ../Include/Vector.h ../../Misc/GC.h
nobj/BIMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/BIMatrix.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h
nobj/BIMatrix.o: ../../Set/Set.h /usr/include/CC/math.h ../Include/Entry.h
nobj/BIMatrix.o: ../Include/SVector.h ../../Set/AssocSet.h
nobj/Banded.o: ../Include/SparseMatrix.h ../Include/Matrix.h
nobj/Banded.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/Banded.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
nobj/Banded.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
nobj/Banded.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
nobj/Banded.o: ../Include/Entry.h ../Include/Banded.h ../Include/Solver.h
nobj/Banded.o: ../../Misc/Timer.h /usr/include/CC/sys/time.h
nobj/Banded.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
nobj/Banded.o: /usr/include/CC/time.h /usr/include/prof.h
nobj/Conj.o: ../Include/Conj.h ../Include/Solver.h ../../Set/SimpleSet.h
nobj/Conj.o: ../../Set/Set.h /usr/include/CC/iostream.h
nobj/Conj.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/Conj.o: ../Include/Entry.h /usr/include/CC/stdio.h ../../Misc/GC.h
nobj/Conj.o: /usr/include/CC/stddef.h ../Include/Vector.h
nobj/Conj.o: /usr/include/CC/math.h ../../Misc/Timer.h
nobj/Conj.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/Conj.o: /usr/include/CC/unistd.h ../Include/Matrix.h
nobj/ConjJacob.o: /usr/include/CC/new.h /usr/include/CC/stddef.h
nobj/ConjJacob.o: /usr/include/CC/std.h /usr/include/CC/_G_config.h
nobj/ConjJacob.o: /usr/include/CC/stdlib.h /usr/include/CC/string.h
nobj/ConjJacob.o: /usr/include/CC/memory.h /usr/include/CC/unistd.h
nobj/ConjJacob.o: /usr/include/CC/stdio.h /usr/include/CC/errno.h
nobj/ConjJacob.o: /usr/include/CC/fcntl.h ../Include/ConjJacob.h
nobj/ConjJacob.o: ../Include/Solver.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/ConjJacob.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/ConjJacob.o: ../Include/Entry.h ../../Misc/GC.h ../Include/Vector.h
nobj/ConjJacob.o: /usr/include/CC/math.h ../../Misc/Timer.h
nobj/ConjJacob.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/ConjJacob.o: ../Include/Matrix.h ../Include/SparseMatrix.h
nobj/DenseMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/DenseMatrix.o: /usr/include/CC/stddef.h ../Include/DenseMatrix.h
nobj/DenseMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
nobj/DenseMatrix.o: ../Include/Vector.h ../../Misc/GC.h
nobj/DenseMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/DenseMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
nobj/DenseMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
nobj/Entry.o: ../Include/Entry.h /usr/include/CC/stdio.h
nobj/Entry.o: /usr/include/CC/_G_config.h ../../Misc/GC.h
nobj/Entry.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Entry.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/Gauss.o: ../Include/Gauss.h ../Include/Matrix.h /usr/include/CC/stdio.h
nobj/Gauss.o: /usr/include/CC/_G_config.h ../Include/Vector.h ../../Misc/GC.h
nobj/Gauss.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Gauss.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/Gauss.o: /usr/include/CC/math.h ../Include/Entry.h
nobj/Gauss.o: ../Include/SparseMatrix.h ../Include/Solver.h
nobj/Gauss.o: ../../Misc/Timer.h /usr/include/CC/sys/time.h
nobj/Gauss.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
nobj/IdentityMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/IdentityMatrix.o: /usr/include/CC/stddef.h ../Include/IdentityMatrix.h
nobj/IdentityMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
nobj/IdentityMatrix.o: ../Include/Vector.h ../../Misc/GC.h
nobj/IdentityMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/IdentityMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
nobj/IdentityMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
nobj/Jacobi.o: ../Include/Jacobi.h ../Include/Matrix.h
nobj/Jacobi.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/Jacobi.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
nobj/Jacobi.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
nobj/Jacobi.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
nobj/Jacobi.o: ../Include/Entry.h ../Include/SparseMatrix.h
nobj/Jacobi.o: ../Include/Solver.h ../../Misc/Timer.h
nobj/Jacobi.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/Jacobi.o: /usr/include/CC/unistd.h
nobj/Matrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/Matrix.o: /usr/include/CC/stddef.h ../Include/Matrix.h
nobj/Matrix.o: /usr/include/CC/stdio.h ../Include/Vector.h ../../Misc/GC.h
nobj/Matrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Matrix.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
nobj/Matrix.o: ../Include/Entry.h
nobj/SOR.o: ../Include/SOR.h ../Include/Matrix.h /usr/include/CC/stdio.h
nobj/SOR.o: /usr/include/CC/_G_config.h ../Include/Vector.h ../../Misc/GC.h
nobj/SOR.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/SOR.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/SOR.o: /usr/include/CC/math.h ../Include/Entry.h
nobj/SOR.o: ../Include/SparseMatrix.h ../Include/Solver.h ../../Misc/Timer.h
nobj/SOR.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/SOR.o: /usr/include/CC/unistd.h
nobj/SVector.o: ../Include/SVector.h ../../Set/AssocSet.h
nobj/SVector.o: ../../Set/SimpleSet.h ../../Set/Set.h
nobj/SVector.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/SVector.o: /usr/include/CC/_G_config.h ../Include/Vector.h
nobj/SVector.o: ../../Misc/GC.h /usr/include/CC/stddef.h
nobj/SVector.o: /usr/include/CC/math.h
nobj/Solver.o: ../Include/Solver.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/Solver.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Solver.o: /usr/include/CC/_G_config.h ../Include/Entry.h
nobj/Solver.o: /usr/include/CC/stdio.h ../../Misc/GC.h
nobj/Solver.o: /usr/include/CC/stddef.h ../Include/Vector.h
nobj/Solver.o: /usr/include/CC/math.h ../../Misc/Timer.h
nobj/Solver.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/Solver.o: /usr/include/CC/unistd.h
nobj/SparseMatrix.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/SparseMatrix.o: /usr/include/CC/stddef.h ../Include/SparseMatrix.h
nobj/SparseMatrix.o: ../Include/Matrix.h /usr/include/CC/stdio.h
nobj/SparseMatrix.o: ../Include/Vector.h ../../Misc/GC.h
nobj/SparseMatrix.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/SparseMatrix.o: ../../Set/SimpleSet.h ../../Set/Set.h
nobj/SparseMatrix.o: /usr/include/CC/math.h ../Include/Entry.h
nobj/SparseMatrix.o: ../Include/YSMP.h ../Include/Solver.h ../../Misc/Timer.h
nobj/SparseMatrix.o: /usr/include/CC/sys/time.h
nobj/SparseMatrix.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
nobj/SparseMatrix.o: ../Include/Jacobi.h ../Include/Banded.h
nobj/SparseMatrix.o: ../Include/Gauss.h ../Include/SOR.h
nobj/SparseMatrix.o: ../Include/ConjJacob.h ../Include/Conj.h
nobj/SparseMatrix.o: /usr/include/CC/ctype.h alleigen.h
nobj/Tridiagonal.o: ../Include/Tridiagonal.h ../Include/Matrix.h
nobj/Tridiagonal.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/Tridiagonal.o: ../Include/Vector.h ../../Misc/GC.h
nobj/Tridiagonal.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Tridiagonal.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h
nobj/Tridiagonal.o: ../../Set/Set.h /usr/include/CC/math.h ../Include/Entry.h
nobj/Vector.o: ../Include/Vector.h ../../Misc/GC.h /usr/include/CC/iostream.h
nobj/Vector.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/Vector.o: /usr/include/CC/stddef.h ../../Set/SimpleSet.h ../../Set/Set.h
nobj/Vector.o: /usr/include/CC/math.h /usr/include/CC/assert.h
nobj/Vector.o: /usr/include/CC/stdio.h /usr/include/CC/ctype.h
nobj/YSMP.o: ../Include/YSMP.h /usr/include/CC/stdlib.h
nobj/YSMP.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
nobj/YSMP.o: ../Include/SparseMatrix.h ../Include/Matrix.h
nobj/YSMP.o: /usr/include/CC/stdio.h ../Include/Vector.h ../../Misc/GC.h
nobj/YSMP.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/YSMP.o: ../../Set/SimpleSet.h ../../Set/Set.h /usr/include/CC/math.h
nobj/YSMP.o: ../Include/Entry.h ../Include/Solver.h ../../Misc/Timer.h
nobj/YSMP.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/YSMP.o: /usr/include/CC/unistd.h
nobj/dysmp.o: /usr/include/f2c.h
