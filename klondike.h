#ifndef __KLONDIKE_H
#define __KLONDIKE_H

#include "card.h"
#include "deck.h"
#include "render.h"

/**
 * Game state of Klondike Solitaire.
 * The reason why we track the source of the hand's cards is twofold:
 * One, we need to know where they come from to give points,
 * Foundation->foundation is not the same as waste->foundation.
 * Two, if we are pulling from the waste, we need to see if we can put it back.
 */
enum klondike_state {
  KS_TABMODE, /**< Otherwise, usually to select a tableau */
  KS_HAND_TAB, /**< We have a card in our hand, its from the tableau */
  KS_HAND_FOUNDATION, /**< It's from the foundation */
  KS_HAND_WASTE /**< It's from the waste */
};

/**
 * The game context for klondike solitaire.
 * Includes all card data, game state, etc. info.
 */
typedef struct {
  /**
   * This is shared memory for pointer casting
   * compatability. Includes the input callback
   * and render stack variables.
   */
  struct render_context render_context;
  
  /**
   * The 7 columns as decks (piles)
   */
  deck_t *tableau[7];
  
  /**
   * The 4 foundations, in order of suit.
   * @see enum suit
   */
  deck_t *foundations[4];
  
  /**
   * The player's hand.
   * len should be non-zero iff. KS_HANDMODE.
   * @see enum klondike_state
   */
  deck_t *hand;
  
  /**
   * Face DOWN cards yet to be moved to waste.
   */
  deck_t *stock;
  
  /**
   * Face UP cards yet to be used in game.
   */
  deck_t *waste;
  
  /**
   * Current state of the game.
   */
  enum klondike_state state;
  
  /**
   * Points according to Microsoft Solitaire conventions.
   */
  int points;
  
  /**
   * Total number of valid moves made.
   */
  int moves;
} klondike_ctx;


/**
 * Attempt to move the hand deck to the tableau column.
 * Under standard klondike rules,
 *  - First card of hand needs to be one less than last card of tableau.
 *  - First card of hand needs to be opposite color of last card of tableau.
 *
 * Switches game state to tab mode on success.
 *
 * @param hand Deck representing the player's hand
 * @param tableau_col The specific column of tableau to place to.
 * @return 0 if successfully moved, 1 if failed
 */
int k_hand_tableau(deck_t *hand, deck_t *tableau_col);


/**
 * Attempt to move the selected tableau card forward into the hand.
 * Works only if game state is tab mode, card isn't obscured, card exists.
 *
 * Switches game state to hand (tableau) mode.
 *
 * @param tableau_col The column to pull from.
 * @param hand The hand to move to.
 * @param index From what index forward are we attempting to pull? (Inclusive)
 * @return 0 if successfully moved, 1 if failed.
 */
int k_tableau_hand(deck_t *tableau_col, deck_t *hand, size_t index);

/**
 * Attempt to move the hand into the foundation.
 * Works only if game state isn't tab mode,
 * hand length is 1, hand card value one less than last foundation card.
 *
 * Switches game state to tab mode on success.
 *
 * @param hand The hand to pull from.
 * @param foundations All four foundations.
 * @return 0 if successfully moved, 1 if failed.
 */
int k_hand_foundation(deck_t *hand, deck_t **foundations);

/**
 * Attempt to move a card from foundation to the hand.
 * Works only if game state is tab mode, foundation len isn't 0.
 *
 * Switches game state to hand (foundation) mode on success.
 *
 * @param foundation The foundation to pull from.
 * @param hand The hand to push to.
 * @return 0 on successful move, 1 if failed.
 */
int k_foundation_hand(deck_t *foundation, deck_t *hand);

/**
 * Attempt to move last card from tableau column to a foundation.
 * Works only if game state is tab mode, foundation rules apply.
 *
 * @param tableau_col The column to pull from.
 * @param foundations All four foundations.
 * @return 0 on successful move, 1 if failed.
 */
int k_tableau_foundation(deck_t *tableau_col, deck_t **foundations);

/**
 * Attempt to move last card from waste into hand.
 * Works only in tab mode.
 *
 * Changes game state to hand (waste).
 *
 * @param waste The waste pile.
 * @param hand The hand to move to.
 * @return 0 on successful move, 1 if failed.
 */
int k_waste_hand(deck_t *waste, deck_t *hand);

/**
 * Attempt to return a waste card to the waste pile, say, if it cannot be played.
 * Works only in hand (waste) mode.
 *
 * Changes game state to tab mode.
 *
 * @param hand The hand to move from.
 * @param waste The waste pile.
 * @return 0 if successful move, 1 if failed.
 */
int k_hand_waste(deck_t *hand, deck_t *waste);

/**
 * Flips three cards from the stock to the waste.
 * Refreshes if the stock is empty.
 */
void k_stock_waste(deck_t *stock, deck_t *waste);

#endif // __KLONDIKE_H
