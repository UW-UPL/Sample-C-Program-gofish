CC := gcc
CFLAGS := -Wall

BINS := \
	deck \
	player \
	game

BINS := $(addsuffix .o, $(BINS))

.PHONY: all clean

all: gofish

clean:
	rm -f $(BINS) gofish

gofish: $(BINS)
	gcc $(BINS) -o gofish

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
