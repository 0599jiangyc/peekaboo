PROG = decaf_reader.o reader #decaf2peekaboo
PROJ_HOME = ../..
IDIR_PEEKABOO = $(PROJ_HOME)/libpeekaboo
LDIR_PEEKABOO = $(PROJ_HOME)/libpeekaboo
LIBS = -lpeekaboo
CC = gcc
OPT = -O2
LIBNAME = capstone
WARNINGS =
CFLAGS = $(WARNINGS) $(OPT) -I$(IDIR_PEEKABOO) -L$(LDIR_PEEKABOO)
CFLAGS2 = -g -Wall

all: $(PROG)

reader: reader.c decaf_reader.o 
	$(CC) reader.c decaf_reader.o -o reader $(CFLAGS2)

decaf_reader.o: decaf_reader.c
	$(CC) $(CFLAGS2) -c decaf_reader.c -o decaf_reader.o

#decaf2peekaboo: decaf2peekaboo.o decaf_reader.o $(LDIR_PEEKABOO)/libpeekaboo.a
#	$(CC) decaf2peekaboo.o decaf_reader.o $(LDIR_PEEKABOO)/libpeekaboo.a -o decaf2peekaboo $(CFLAGS) $(LIBS) -l$(LIBNAME)

$(LDIR_PEEKABOO)/libpeekaboo.a:
	(cd $(LDIR_PEEKABOO) && $(MAKE))

.PHONY: clean
