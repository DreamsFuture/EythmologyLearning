CC = g++
INCLUDES = -I.. -I../Matrix/Include
CFLAGS = -g -Wall $(INCLUDES) -DDEBUG

LSRCS = Opt.c StdOpt.c DOTOpt.c
LOBJS = $(LSRCS:.c=.o)

DOBJS = DOT/ddot1.o DOT/ddot2.o DOT/ddot3.o DOT/ddot4.o DOT/ddot5.o DOT/ddot6.o

libOpt.a:	$(LOBJS) $(DOBJS) f2c/*.o
	ar rv libOpt.a $(LOBJS) $(DOBJS) f2c/*.o
	ranlib libOpt.a

depend:
	makedepend $(LSRCS) $(TSRCS) $(INCLUDES) -I/usr/include/CC

clean:
	-rm $(LOBJS) libOpt.a $(TOBJS)

# DO NOT DELETE THIS LINE -- make depend depends on it.

Opt.o: Opt.h /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
Opt.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
Opt.o: /usr/include/CC/math.h /usr/include/CC/stdlib.h
Opt.o: /usr/include/CC/stddef.h /usr/include/CC/assert.h
StdOpt.o: StdOpt.h Opt.h /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
StdOpt.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
StdOpt.o: /usr/include/CC/math.h
DOTOpt.o: DOTOpt.h /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
DOTOpt.o: /usr/include/CC/math.h Opt.h /usr/include/CC/iostream.h
DOTOpt.o: /usr/include/CC/streambuf.h /usr/include/CC/stdlib.h
DOTOpt.o: /usr/include/CC/stddef.h
