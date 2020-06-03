PROG = read_trace write_trace cut_trace
PROJ_HOME = .
IDIR_PEEKABOO = $(PROJ_HOME)/libpeekaboo
LDIR_PEEKABOO = $(PROJ_HOME)/libpeekaboo
PARSER_DIR = $(PROJ_HOME)/plugins/trace_parsers
LIBS = -lpeekaboo
CC = gcc
OPT = -O2
LIBNAME = capstone
WARNINGS =
CFLAGS  = $(WARNINGS) $(OPT) -I$(IDIR_PEEKABOO) -L$(LDIR_PEEKABOO) -I$(PARSER_DIR) -L$(PARSER_DIR)

all: $(PROG)

debug: CFLAGS += -DDEBUG -g
debug: $(PROG)

read_trace: read_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a
	$(CC) read_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a -o read_trace $(CFLAGS) $(LIBS) -l$(LIBNAME)

write_trace: write_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a $(PARSER_DIR)/peekaboo_parser.a
	$(CC) write_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a $(PARSER_DIR)/peekaboo_parser.a -o write_trace $(CFLAGS) $(LIBS) -l$(LIBNAME)

cut_trace: cut_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a
	$(CC) cut_trace.o $(LDIR_PEEKABOO)/libpeekaboo.a -o cut_trace $(CFLAGS) $(LIBS) -l$(LIBNAME)

$(LDIR_PEEKABOO)/libpeekaboo.a:
	(cd $(LDIR_PEEKABOO) && $(MAKE))

.PHONY: clean

clean:
	rm -f *.o *.a *.gch $(PROG)
	(cd $(LDIR_PEEKABOO) && $(MAKE) clean)
