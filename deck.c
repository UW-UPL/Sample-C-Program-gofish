#include <stdio.h> /** Needed for printf */
#include <string.h> /** Needed for memmove */
#include <stdlib.h> /** Needed for malloc */

#include "gofish.h"

/**
 * Allocate a new deck for the game. Returns a pointer to a new
 * deck object on success, NULL on failure.
 */
struct deck* deck_alloc(void)
{
	/* Allocate the deck on the heap using malloc */
	struct deck* d = malloc(sizeof(struct deck));

	/* Initlize the deck (this is call zeroing) */
	memset(d, 0, sizeof(struct deck));
	d->top = -1; /* The deck is empty */

	/* Return the deck as the result */
	return d;
}

/**
 * Fill a deck with the 52 game cards. Takes the deck, d, as
 * the parameter.
 */
void deck_populate(struct deck* d)
{
	rank_t r;
	suit_t s;

	/* Loop through all of the ranks */
	for(r = R2; r <= RANK_MAX; r++)
	{
		/* For each rank, add one card of each suit */
		for(s = CLUBS; s < SUIT_MAX; s++)
		{
			/* the card we are adding to the deck */
			struct card c;
			c.r = r; /* Assign rank */
			c.s = s; /* Assign suit */

			/* Add the card to the deck */
			if(deck_put(d, &c))
			{
				printf("gofish: deck full!\n");
				exit(1);
			}
		}
	}

	/* Adjust the top index */
	d->top = DECK_SIZE - 1;
}

/**
 * Puts the given card, c, in the given deck, d. Returns 0 on
 * success, nonzero otherwise.
 */
int deck_put(struct deck* d, struct card* c)
{
	/* Is this deck full? */
	if(d->top == DECK_SIZE - 1)
	{
		/* Can't add to a full deck */
		return -1;
	}

	/* Increment the top index */
	d->top++;

	/* Put the card at the top index */
	memmove(c, d->cards + d->top, sizeof(struct card));

	return 0; /* Return success */
}

/**
 * Draw a card from the given deck, d. Returns a pointer to
 * a card on success (there were card(s) in the deck). Returns
 * NULL on failure (the deck was empty).
 */
struct card* deck_draw(struct deck* d)
{
	/* Is this deck empty? */
	if(d->top < 0)
	{
		/* Return failure. */
		return NULL;
	}

	/* Create a new card */
	struct card* c = malloc(sizeof(struct card));

	/* Copy the card in the deck */
	memmove(d->cards + d->top, c, sizeof(struct card));

	/* Delete the card from the deck */
	memset(d->cards + d->top, 0, sizeof(struct card));
	
	/* Adjust the top index */
	d->top--;

	return c;
}

/**
 * Check to see if a deck, d, contains any cards of rank, r.
 * If there exists such a card in the deck, it is removed
 * from the deck and returned. Returns NULL if there is no
 * such card in the deck.
 */
struct card* deck_contains(struct deck* d, rank_t r)
{
	int x;

	/* Loop through the cards in the deck */
	for(x = 0;x <= d->top;x++)
	{
		if(d->cards[x].r == r)
		{
			/* Found a card with a matching rank! */
			struct card* c = malloc(sizeof(struct card));

			/* Copy the card from the deck */
			memmove(c, d->cards + x, sizeof(struct card));

			/* Fix the deck so there isn't an empty space */
			for(;x <= d->top - 1;x++)
			{
				/* Move the next card to the current spot */
				memmove(d->cards + x, d->cards + x + 1, 
					sizeof(struct card));
			}

			/* Adjust top of deck */
			d->top--;

			/* Return the card we found */
			return c;
		}
	}

	return NULL;
}
