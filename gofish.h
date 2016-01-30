/**
 * The below C macros are called header guards. It prevents the
 * following content from being expanded more than once in a
 * given source file. For example, if you include this header
 * in a source file and you also include another header that
 * includes this header, it will be included twice, therefore
 * you will end up with more than one declaration of everything
 * that is defined here. This header guard works as follows:
 *
 * 
 * 1) _GOFISH_H_ is undefined.
 * 2) The c preprocessor starts processing your source file 
 *        and encounters: #include "gofish.h"
 * 3) The c preprocessor starts expanding gofish.h into your
 *        source file and encounters: #ifdef _GOFISH_H_
 * 4) Is _GOFISH_H_ defined? NO
 * 5) The c preprocessor encounters: define _GOFISH_H_ and
 *        now _GOFISH_H_ is defined (with no value).
 * 6) The rest of the gofish.h header is expanded into the
 *        source file
 *
 * Now suppose in your source file, you include file.h, which
 * also includes this header.
 *
 * 7) The c preprocessor encounters: "#include <file.h>"
 * 8) The c preprocessor starts expanding file.h and
 *           encounters: #include "gofish.h"
 * 9) Now, the c preprocessor starts expanding gofish.h for a second 
 *           time and encounters: #ifndef _GOFISH_H_
 * 10) Is _GOFISH_H_ defined? YES - we have already processes gofish.h
 *
 * Final notes:
 *
 * Macro expansion works on a per source file basis. This means that
 * each source file starts with a clean slate of macros. When the
 * c preprocessor works on each individual file, each file will
 * get an expansion of the header gofish.h.
 *
 */
#ifndef _GOFISH_H_
#define _GOFISH_H_
/** ^^ Header guard */

/* How slow should the dialog go? This is measured in seconds */
#define DIALOG_SPEED 1

/**
 * The suit of a given card
 */
enum suit 
{
	CLUBS = 1, SPADES, 
	HEARTS, DIAMONDS
};
#define SUIT_MAX 4
typedef int suit_t;

/* The names of the ranks (defined in game.c) */
extern const char* rank_names[];

/**
 * The values of any given card. Ace is high.
 */
enum rank 
{
	R2 = 2, R3, R4, R5, R6, R7,
	R8, R9, R10, RJ, RQ, RK, RA
};
#define RANK_MAX 14
typedef int rank_t;

/**
 * Represents a card in a given deck of standard
 * playing cards.
 */
struct card
{
	suit_t s; /* The suit of a card */
	rank_t r; /* The rank/type of card */
};

/**
 * Represents the deck of cards in the
 * middle for drawing. Can also represent
 * a player's hand.
 */
#define DECK_SIZE 52
struct deck
{
	int top; /* The index of the top card (-1 means deck is empty) */
	struct card cards[DECK_SIZE]; /* The cards in the deck (52 max) */
};


/**
 * How many characters can a player's name have? 
 */
#define NAME_MAX 64

/**
 * A game player. This can either be a
 * computer player or a human player.
 */
struct player 
{
	struct deck d; /* The player's hand */
	char name[NAME_MAX]; /* The player's name */
	int books; /* How many sets has the player acquired? */

	/** Functions for player here */

	/**
	 * When it is the player's turn, this function is called. A pointer
	 * to the player who's going is provided as p. The player will then
	 * place the rank that is guessed in guess_rank. The player will also
	 * place a pointer to the player who he is asing for.
	 */
	int (*my_turn)(struct player* p, /*The player that is currently going*/
		rank_t* guess_rank, /* The rank we are guessing */
		struct player** guess_player); /* The player we are asking */
};
/* How many player can play in a game? */
#define PLAYERS_MAX 12

/** The players in the game (defined in game.c) */
extern struct player game_players[PLAYERS_MAX];

/** The amount of players that are actually playing (defined in game.c)*/
extern int player_count;

/** The functions for manipulating decks.*/

/**
 * Fill a deck with the 52 game cards. Takes the deck, d, as
 * the parameter.
 */
void deck_populate(struct deck* d);

/**
 * Shuffle the given deck, d.
 */
void deck_shuffle(struct deck* d);

/**
 * Puts the given card, c, in the given deck, d. Returns 0 on
 * success, nonzero otherwise.
 */
int deck_put(struct deck* d, struct card* c);

/**
 * Draw a card from the given deck, d. Returns a pointer to
 * a card on success (there were card(s) in the deck). Returns
 * NULL on failure (the deck was empty).
 */
struct card* deck_draw(struct deck* d);

/**
 * Check to see if a deck, d, contains any cards of rank, r.
 * If there exists such a card in the deck, it is removed
 * from the deck and returned. Returns NULL if there is no
 * such card in the deck.
 */
struct card* deck_contains(struct deck* d, rank_t r);

/**
 * Check to see if the deck contains a completed set of cards.
 * Return the rank of the card that has a complete set, and
 * removes all of those cards from the deck. Returns -1 if
 * no set was found.
 */
rank_t deck_get_set(struct deck* d);

/** Functions for manipulating players */

/**
 * Setup the given player with the given values. The name will
 * be copied into the player struct. The player will be setup
 * with the proper playing functions based on whether human
 * is 1 (TRUE) or 0 (FALSE).
 */
void player_setup(struct player* p, const char* name, int human);

/**
 * Logic function for doing a human player's turn. The human
 * player will be prompted for a rank and a player's name.
 * returns 0 on success.
 */
int human_player_turn(struct player* p,
        rank_t* guess_rank,
        struct player** guess_player);

/**
 * Logic function for doing a computer player's turn. The computer
 * players will basically do everything as random as possible. They
 * will ask random players for cards that they have in their hand.
 * Returns 0 on success, -1 if a move could not be generated.
 */
int computer_player_turn(struct player* p,
        rank_t* guess_rank,
        struct player** guess_player);

#endif
