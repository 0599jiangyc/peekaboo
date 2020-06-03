TARGET = peekaboo_parser.a
PEEKABOOLIB_DIR = ../../libpeekaboo
CC = gcc
OPT = -O2
LIBS = -lpeekaboo
WARNINGS =
ARCH_DIR = ./arch
IDIR = ../../libpeekaboo
LDIR = ../../libpeekaboo
CFLAGS  = $(WARNINGS) -I$(IDIR) -L$(LDIR)

all: $(TARGET)

peekaboo_parser.a: peekaboo_parser.o $(PEEKABOOLIB_DIR)/libpeekaboo.a
	ar rcs $(TARGET) peekaboo_parser.o
	ranlib peekaboo_parser.a

peekaboo_parser.o: peekaboo_parser.c peekaboo_parser.h $(PEEKABOOLIB_DIR)/libpeekaboo.a
	$(CC) -c peekaboo_parser.c peekaboo_parser.h $(CFLAGS) $(PEEKABOOLIB_DIR)/libpeekaboo.a $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *.a *.gch $(ARCH_DIR)/*.o $(ARCH_DIR)/*.gch
