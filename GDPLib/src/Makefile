
CC = gcc
CFLAGS = -I.

DEPS = gdpl.h

OBJ =   main.o kontroller.o konkurranse.o person.o par.o test.o test_person.o test_konkurranse.o test_kontroller.o test_par.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o *~ core test test.exe.stackdump



