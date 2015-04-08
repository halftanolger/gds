#
# Makefile for GDSLib -testklient
#

CC = gcc
FLAGS = -Wall
INCLUDES = -I ../GDPLib/src/ -I src/

all: gubb

gubb:    ../GDPLib/src/konkurranse.c ../GDPLib/src/kontroller.c ../GDPLib/src/log.c ../GDPLib/src/modell.c src/util.c src/main.c ../GDPLib/src/par.h ../GDPLib/src/person.c 
	$(CC) $(FLAGS) $(INCLUDES) ../GDPLib/src/modell.c ../GDPLib/src/konkurranse.c ../GDPLib/src/kontroller.c ../GDPLib/src/log.c src/util.c src/main.c ../GDPLib/src/par.c ../GDPLib/src/person.c -o gubb
    
clean:
	rm gubb.exe 

