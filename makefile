CC = gcc
CFLAGS = -I. -g -Wall -W
IDIR = include

LEX = flex
DFT_PARSER_H = parser.h
YACC = bison --defines=$(IDIR)/$(DFT_PARSER_H) -y

TDIR = tests
SDIR = src
ODIR = src/obj

_TOBJ = http_parser_test.o
TOBJ = $(patsubst %, $(ODIR)/%, $(_TOBJ))

_OBJ = ht.o parser.o scanner.o util.o net.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

_SOBJ = ht.o parser.o scanner.o util.o server.o net.o
SOBJ = $(patsubst %, $(ODIR)/%, $(_SOBJ))

_DEP = ht.h util.h http.h net.h
DEP = $(patsubst %, $(IDIR)/%, $(_DEP))

$(ODIR)/%.o: $(SDIR)/%.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $<

$(ODIR)/%_test.o: $(TDIR)/%_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

server: $(SOBJ)
	$(CC) $(CFLAGS) -o $@ $(SOBJ)

test: $(OBJ) $(TOBJ)
	for i in $(TOBJ); do \
		$(CC) $(CFLAGS) -o $@ $(OBJ) $$i && ./$@; \
	done

scanner.o: scanner.l

parser.o: parser.y

.PHONY: clean

clean:
	rm -rf $(ODIR)/*.o $(IDIR)/$(DFT_PARSER_H)