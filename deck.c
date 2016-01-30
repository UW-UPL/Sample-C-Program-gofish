/**
 * This game is provided as an example of good use of the C programming
 * language for beginners. We assume you are familiar with simple
 * programming constructs like for loops and pointers. Complicated
 * functions are well documented. This game was originally written
 * by Riccardo Mutschlechner <riccardo@cs.wisc.edu> and Nik Ingrassia,
 * but was rewritten from scratch.
 *
 * Author: John Detter <jdetter@wisc.edu>
 * 
 */

#include <stdio.h> /** Needed for printf */
#include <string.h> /** Needed for memmove */
#include <stdlib.h> /** Needed for malloc and rand */

#include "gofish.h"

/**
 * Fill a deck with the 52 game cards. Takes the deck, d, as
 * the parameter.
 */
void deck_populate(struct deck* d)
{
	d->top = -1;
	rank_t r;
	suit_t s;

	/* Loop through all of the ranks */
	for(r = R2; r <= RANK_MAX; r++)
	{
		/* For each rank, add one card of each suit */
		for(s = CLUBS; s <= SUIT_MAX; s++)
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
}

/**
 * Shuffle the given deck, d.
 */
void deck_shuffle(struct deck* d)
{
	/* Just to a limited swapping randomization */
	int x;
	for(x = 0;x < 10000;x++)
	{
		/* Pick 2 indexes */
		int index_1 = rand() % (d->top + 1);
		int index_2 = rand() % (d->top + 1);

		/* Swap the two values at the given indexes */
		struct card tmp;
		/* Save index_1 in tmp */
		memmove(&tmp, d->cards + index_1, sizeof(struct card));
		/* index_1 = index_2*/
		memmove(d->cards + index_1, d->cards + index_2, sizeof(struct card));
		/* index_2 = tmp */
		memmove(d->cards + index_2, &tmp, sizeof(struct card));
	}
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
		printf("go fish: tried to add card to full deck!\n");
		exit(1);
	}

	/* Increment the top index */
	d->top++;

	/* Put the card at the top index */
	memmove(d->cards + d->top, c, sizeof(struct card));

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
	memmove(c, d->cards + d->top, sizeof(struct card));

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

/**
 * Check to see if the deck contains a completed set of cards.
 * Return the rank of the card that has a complete set, and
 * removes all of those cards from the deck. Returns -1 if
 * no set was found.
 */
rank_t deck_get_set(struct deck* d)
{
	/* Keep track of the cards we have */
	char card_counts[RANK_MAX + 1];
	memset(card_counts, 0, RANK_MAX + 1);

	int x;
	for(x = 0;x <= d->top;x++)
	{
		/* Increment card counts for this card rank */
		card_counts[d->cards[x].r]++;
	}

	/* Did we find any sets? */
	for(x = R2;x <= RANK_MAX;x++)
	{
		if(card_counts[x] >= 4)
		{
			/** We found a set! */
		
			/* Remove all of these cards */
			struct card* c = NULL;
			do
			{
				/* Find and remove a card */
				c = deck_contains(d, x);
	
				/* Did we get a card? */
				if(c)
				{
					/* Free the card*/
					free(c);
				}
			} while(c != NULL);

			/* Return this rank */
			return x;
		}
	}

	/* We found no complete sets */
	return -1;
}
