#ifndef __RENDER_H
#define __RENDER_H

#include "deck.h"

/**
 * The generic render context for card games in CSOL.
 * This structure contains the input callback function used
 * by the input function and provided by the game mode.
 * This structure also contains _the_ deck data used by any
 * game mode.
 */
struct render_context {
  /**
   * The input callback function.
   * @return 0 if successful operation, 1 if user error (flash)
   */
  int (*input_callback)(struct render_context *ctx, int ch);
  /**
   * The deck data for the game mode.
   * Array of length `decks`.
   */
  deck_t *deck_data;
  /**
   * The length of `deck_data`
   */
  int decks;

  /**
   * Minimum screen height required to play.
   * Measured in card height.
   */
  float minheight;
  /**
   * Minimum screen width required to play.
   * Measured in card width.
   */
  float minwidth;
};

void render(deck_t *deck_data, int decks);

#endif //__RENDER_H
