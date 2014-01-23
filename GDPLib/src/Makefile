
CC = gcc
CFLAGS = -I.

DEPS = gdpl.h

OBJ =   test.o main.o kontroller.o konkurranse.o person.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ core test 



