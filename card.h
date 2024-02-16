#ifndef __CARD_H
#define __CARD_H

/**
 * A card's suit.
 */
enum suit {
  SUIT_SPADE,   /**< Spades, printed S */
  SUIT_CLUB,    /**< Clubs, printed C */
  SUIT_DIAMOND, /**< Diamonds, printed D */
  SUIT_HEART    /**< Hearts, printed H */
};


/**
 * A card's state. Options include default, face-down, selected, and both.
 * These states are intended to be controled as bits,
 * e.g. `CARD_BOTH = CARD_SEL | CARD_DOWN`.
 */
enum card_state {
  CARD_NONE = 0b00, /**< Shall print with standard colors */
  CARD_DOWN = 0b01, /**< Shall print the back pattern and colors */
  CARD_SEL  = 0b10, /**< Shall print inverted colors, standard face */
  CARD_BOTH = 0b11  /**< Shall print inverted colors, back face */
};

/**
 * Easy definitions for non-standard values.
 */
enum special_values {
  VAL_JOK3  = 0,  /**< Atypical in western decks. Printed !
		   * Assigned Black color (Spades)
		   */
  VAL_JACK  = 11, /**< Jack. Printed J */
  VAL_QUEEN = 12, /**< Queen. Printed Q */
  VAL_KING  = 13, /**< King. Printed K */
  VAL_JOK1  = 14, /**< Black Joker. Printed ?
		   * Assigned Black color (Spades)
		   */
  VAL_JOK2  = 15  /**< Red Joker. Printed ?
		   * Assigned Red color (Diamonds)
		   */
};

/**
 * The base playing card structure.
 * Variables are defined as bit-fields validly within restriction.
 */
typedef struct {
  /**
   * Suit of the card.
   */
  enum suit suit : 2;
  /**
   * State of the card.
   */
  enum card_state state : 2;
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
 * @param state The status of the card.
 * @see enum suit
 * @see enum card_state
 * @see enum special_values
 * @return The card.
 */
card_t newcard(int value, int suit, int state);

/**
 * Prints the card information to a string buffer.
 * More specifically, buffer must be of size 3.
 * Output follows conventions laid out by suit defs
 * and special values.
 *
 * (Example) " KH", "10C" " 4D" " ??" " !!"
 * @param buf A string buffer of size 3 or greater.
 * @param card The card to print.
 * @return 0 on success, 1 on failure
 * @see enum suit
 * @see enum special_values
 */
int sprintname(char *buf, card_t card);



#endif // __CARD_H
