#ifndef __CARD_H
#define __CARD_H

#define vs_to_index(value, suit) ((suit) * 13 + (value) - 1)

/**
 * A card's suit.
 */
enum suit {
  SUIT_HEART,   /**< Hearts, printed H */
  SUIT_CLUB,    /**< Clubs, printed C */
  SUIT_DIAMOND, /**< Diamonds, printed D */
  SUIT_SPADE,   /**< Spades, printed S */
};


/**
 * Easy definitions for non-standard values.
 */
enum special_values {
  VAL_JOK3  = 0,  /**< Atypical in American decks. Printed !
		   * Assigned Black color (Spades)
		   */
  VAL_JACK  = 11, /**< Jack. Printed J */
  VAL_QUEEN = 12, /**< Queen. Printed Q */
  VAL_KING  = 13, /**< King. Printed K */
  VAL_JOK1  = 14, /**< Red Joker. Printed ?
		   * Assigned Red color (Hearts)
		   */
  VAL_JOK2  = 15  /**< Black Joker. Printed ?
		   * Assigned Black color (Clubs)
		   */
};

/**
 * The base playing card structure.
 */
typedef struct {
  /**
   * Suit of the card.
   */
  enum suit suit : 2;
  /**
   * Value, including special values, of the card.
   * @see enum special_values
   */
  unsigned int value : 4;
} card_t;

/**
 * Generates a new card.
 * @param value The value of the card.
 * @param suit The suit of the card.
 * @see enum suit
 * @see enum card_state
 * @see enum special_values
 * @return The card.
 */
card_t newcard(int value, int suit);

/**
 * Prints the card information to a string buffer.
 * More specifically, buffer must be of size 3.
 * Output follows conventions laid out by suit defs
 * and special values.
 *
 * (Example) " KH", "10C" " 4D" " ??" " !!"
 * @param buf A string buffer of size 3 or greater.
 * @param card The card to print.
 * @see enum suit
 * @see enum special_values
 */
void sprintname(char *buf, card_t card);



#endif // __CARD_H
