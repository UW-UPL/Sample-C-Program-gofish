all: gofish

gofish: main.o deck.o player.o card.o
	gcc main.o deck.o player.o card.o -o gofish

main.o: main.c
	gcc -c main.c

deck.o: deck.c deck.h card.h player.h
	gcc -c deck.c

player.o: player.c player.h deck.h
	gcc -c player.c

card.o: card.c card.h
	gcc -c card.c