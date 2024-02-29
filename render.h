#ifndef __RENDER_H
#define __RENDER_H

#include "deck.h"


struct render_context {
  int (*input_callback)(struct render_context *ctx, int ch);
  deck_t *deck_data;
  int decks;

  float minheight;
  float minwidth;
};

void render(deck_t *deck_data, int decks);

#endif //__RENDER_H
