CC := gcc
CFLAGS := -Wall -ggdb

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
	@echo ""
	@echo "The game compiled with no build problems."
	@echo "Use ./gofish to play!"

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
