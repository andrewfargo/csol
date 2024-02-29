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
  KS_TABMODE,         /**< Otherwise, usually to select a tableau */
  KS_HAND_TAB,        /**< We have a card in our hand, its from the tableau */
  KS_HAND_FOUNDATION, /**< It's from the foundation */
  KS_HAND_WASTE       /**< It's from the waste */
};

/**
 * The game context for klondike solitaire.
 * Includes all card data, game state, etc. info.
 */
struct klondike_context {
  /**
   * This is shared memory for pointer casting
   * compatability. This includes ALL deck data,
   * callback function. All other deck
   * data pointers are from this context's array.
   */
  struct render_context render_context;

  /**
   * The 7 columns as decks (piles)
   */
  deck_t *tableau;

  /**
   * The 4 foundations, in order of suit.
   * @see enum suit
   */
  deck_t *foundations;

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
};

/**
 * Create the render_context and klondike_context for a new game of
 * klondike solitaire.
 * @return A generic render context initialized as a klondike context
 */
struct render_context *klondike_initialize();

/**
 * Destroy a klondike_context instance.
 * @param A render context that was initialized as a klondike context
 */
void klondike_destroy(struct render_context *ctx);

/**
 * Interpret input for klondike solitaire.
 * It is safe to assume that `ctx` is klondike_context.
 * @param ctx A klondike context instance
 * @param ch An NCurses character
 * @return 0 if successful operation, 1 if user error
 */
int klondike_input(struct render_context *ctx, int ch);

#endif // __KLONDIKE_H
