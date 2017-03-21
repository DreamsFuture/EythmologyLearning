OBJS = GC.o Timer.o
CC = g++
INCLUDES = -I. -I..
CFLAGS = -g $(INCLUDES) -O2
LSRCS = GC.c Timer.c 
LDFLAGS = -L.
LOADLIBES = -lMisc
NOBJS := $(foreach obj,$(OBJS),nobj/$(obj))
POBJS := $(foreach obj,$(OBJS),pobj/$(obj))

all:	libMisc.a

profiled:	libMisc_p.a

libMisc.a:	$(NOBJS)
	ar rv libMisc.a $(NOBJS)
	ranlib libMisc.a

libMisc_p.a:	$(POBJS)
	ar rv libMisc_p.a $(POBJS)
	ranlib libMisc_p.a

nobj/%.o:	%.c
	@if [ ! -d nobj ]; then mkdir nobj; fi
	$(CC) -c $(CFLAGS) $< -o $@

pobj/%.o:	%.c
	@if [ ! -d pobj ]; then mkdir pobj; fi
	$(CC) -c -pg $(CFLAGS) $< -o $@

depend:
	touch tmp1 tmp2
	makedepend $(INCLUDES) -I$(CPPINC)
	makedepend -ppobj/ $(LSRCS) $(INCLUDES) -I$(CPPINC) -f tmp1
	makedepend -pnobj/ $(LSRCS) $(INCLUDES) -I$(CPPINC) -f tmp2
	cp Makefile Makefile.tmp
	cat Makefile.tmp tmp1 tmp2 > Makefile
	-rm Makefile.tmp tmp1 tmp2 tmp1.bak tmp2.bak

clean:
	-rm *.o *.a $(POBJS) $(NOBJS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
# DO NOT DELETE THIS LINE -- make depend depends on it.

pobj/GC.o: GC.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/GC.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
pobj/GC.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/assert.h
pobj/GC.o: /usr/include/CC/stdio.h /usr/include/CC/new.h
pobj/GC.o: /usr/include/CC/std.h /usr/include/CC/stdlib.h
pobj/GC.o: /usr/include/CC/string.h /usr/include/CC/memory.h
pobj/GC.o: /usr/include/CC/unistd.h /usr/include/CC/errno.h
pobj/GC.o: /usr/include/CC/fcntl.h
pobj/Timer.o: Timer.h /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/Timer.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
pobj/Timer.o: /usr/include/CC/unistd.h
# DO NOT DELETE THIS LINE -- make depend depends on it.

nobj/GC.o: GC.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/GC.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
nobj/GC.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/assert.h
nobj/GC.o: /usr/include/CC/stdio.h /usr/include/CC/new.h
nobj/GC.o: /usr/include/CC/std.h /usr/include/CC/stdlib.h
nobj/GC.o: /usr/include/CC/string.h /usr/include/CC/memory.h
nobj/GC.o: /usr/include/CC/unistd.h /usr/include/CC/errno.h
nobj/GC.o: /usr/include/CC/fcntl.h
nobj/Timer.o: Timer.h /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/Timer.o: /usr/include/CC/sys/time.h /usr/include/CC/sys/resource.h
nobj/Timer.o: /usr/include/CC/unistd.h
