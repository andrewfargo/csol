#ifndef __KLONDIKE_H
#define __KLONDIKE_H

#include "card.h"
#include "deck.h"
#include "render.h"

enum klondike_state {
  KS_TABMODE,         
  KS_HAND_TAB,        
  KS_HAND_FOUNDATION, 
  KS_HAND_WASTE       
};

struct klondike_context {
  struct render_context render_context;

  deck_t *tableau;
  deck_t *foundations;

  deck_t *hand;
  deck_t *stock;
  deck_t *waste;

  enum klondike_state state;
};

struct render_context *klondike_initialize();

void klondike_destroy(struct render_context *ctx);

int klondike_input(struct render_context *ctx, int ch);

#endif // __KLONDIKE_H
