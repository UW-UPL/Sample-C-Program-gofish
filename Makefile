CC := gcc
CFLAGS := -Wall

BINS := \
	main \
	deck \
	player \
	card

BINS := $(addsuffix .o, $(BINS))

.PHONY: all clean

all: gofish

clean:
	rm -f $(BINS) gofish

gofish: $(BINS)
	gcc main.o deck.o player.o card.o -o gofish

%.o: %.c
	$(CC) $(CFLAGS) -o $< $@
