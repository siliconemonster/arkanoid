CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
#CFLAGS=-g -Wall -D_REENTRANT
IMAGE_FLAGS=-lSDL2_image
MUSIC_FLAGS=-lSDL2_mixer
FONT_FLAGS=-lSDL2_ttf
MATHFLAGS=-lm
SFLAGS=-lSDL2
SOURCES=skynoid.c
BINARIES=Skynoid

#all: ex0 ex1 ex2 ex3 ex4 ex5 ex6 ex7 grafico texto
all: $(BINARIES)

Skynoid: skynoid.c
	$(CC) -o Skynoid skynoid.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(MUSIC_FLAGS) $(MATHFLAGS) $(FONT_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out
