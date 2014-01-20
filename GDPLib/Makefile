
CC = gcc
CFLAGS = -I.

ODIR = build

DEPS = src/gdpl.h

_OBJ = src/main.o src/test.o src/controller.o src/util.o src/competition.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/src/*.o *~ core $(INCDIR)/*~ bin/test 



