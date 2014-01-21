
CC = gcc
CFLAGS = -I.

DEPS = gdpl.h

OBJ =   kontroller.o konkurranse.o main.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ core test 



