#ifndef __RENDER_H
#define __RENDER_H

#include "deck.h"

enum render_signal {
  SIGNAL_NONE,
  SIGNAL_QUIT,
  SIGNAL_RENDER_ALL
};  

struct render_context {
  int (*input_callback)(struct render_context *ctx, int ch);
  deck_t *deck_data;
  int decks;

  int deck_select;
  int card_select;

  enum render_signal signal;
  
  float minheight;
  float minwidth;
};

void render(struct render_context *ctx);

#endif //__RENDER_H
