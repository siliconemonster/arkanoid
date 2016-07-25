CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
MUSIC_FLAGS=-lSDL2_mixer
MATHFLAGS=-lm
SFLAGS=-lSDL2
SOURCES=arkanoid02.2.c
BINARIES=arkanoid02

#all: ex0 ex1 ex2 ex3 ex4 ex5 ex6 ex7 grafico texto
all: $(BINARIES)

arkanoid02: arkanoid02.2.c
	$(CC) -o arkanoid02 arkanoid02.2.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MUSIC_FLAGS) $(MATHFLAGS) 

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out
