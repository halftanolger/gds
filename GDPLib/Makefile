#
# Makefile for GDSLib -testklient
#

CC = gcc
CFLAGS = -Wall -std=c89
INCLUDES = -I src/

all: gds

gds:    src/konkurranse.c src/kontroller.c src/log.c src/modell.c src/main.c src/par.h src/person.c 
	$(CC) $(CFLAGS) $(INCLUDES) src/modell.c src/konkurranse.c src/kontroller.c src/log.c src/main.c src/par.c src/person.c -o gds
    
clean:
	rm gds.exe

