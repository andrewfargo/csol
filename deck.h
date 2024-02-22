#ifndef __DECK_H
#define __DECK_H

#include <stdlib.h>
#include "card.h"



/**
 * A deck (or generic pile) of cards.
 * Contains an array of cards, the maximum count it occupies,
 * and how much of that count (0 meaning none) is valid.
 * 
 * The deck operates as a stack, therefore drawing from
 * anywhere but the end is costly, and should be avoided.
 */
typedef struct {
  /**
   * The array of cards.
   */
  card_t *data;
  /**
   * The maximum valid length of data.
   */
  size_t maxlen;
  /**
   * The current height of the deck.
   */
  size_t len;
  /**
   * At which point do we start showing card faces?
   * Note: No solitaire game I could find regularly shows a
   * card beneath an obscured card. This system will need to
   * be reworked if one is found.
   */
  size_t obscure_until;
} deck_t;

/**
 * Creates a new, empty deck of specified length.
 * @param maxlen The maximum size of the deck.
 * @return The deck, allocated to memory.
 * @see destroydeck
 */
deck_t *newdeck(size_t maxlen);

/**
 * Creates a standard deck of playing cards.
 * @param jokers Should the deck have 2 jokers?
 * @return A standard deck in standard order.
 */
deck_t *newdeck_std(bool jokers);

/**
 * Deallocates deck specified.
 * @param deck The deck to destroy.
 * @see newdeck
 */
void destroydeck(deck_t *deck);

/**
 * Shuffles the deck reasonably randomly.
 * @param deck The deck to shuffle.
 */
void shuffledeck(deck_t *deck);

/**
 * Pops the last nth items from one deck to another.
 * THIS OPERATION PRESERVES ORDER.
 *
 * (Pseudo-Example) [1 2 3 4] [1 2] -> [1] [1 2 2 3 4]
 * @param from The deck that loses cards.
 * @param to The deck that gains cards.
 * @param n The amount of cards to move.
 * @return 1 if pop is not possible (n too large), 0 otherwise.
 */
int npop(deck_t *from, deck_t *to, size_t n);

/**
 * Print the contents of a deck, noting obscured cards.
 * @param deck The deck to print.
 */
void debug_printdeck(deck_t *deck);



#endif // __DECK_H