#include "klondike.h"

#include "card.h"
#include "config.h"
#include "deck.h"
#include "render.h"

void k_deck_init(deck_t *tableau, deck_t *foundations, deck_t *hand,
                 deck_t *stock, deck_t *waste) {
  for (int i = 0; i < 7; i++) {
    newdeck(&(tableau[i]), 13);
    tableau[i].obscure_until = i;
    tableau[i].direction = DECK_DIRECTION_DOWN;
    tableau[i].window = place_deck(1, i, 20 * (2.0 / CARD_HEIGHT), 1);
    tableau[i].modified = true;
  }
  for (int i = 0; i < 4; i++) {
    newdeck(&(foundations[i]), 13);
    foundations[i].obscure_until = 0;
    foundations[i].direction = DECK_DIRECTION_NONE;
    foundations[i].window = place_deck(0, i + 3, 1, 1);
    foundations[i].modified = true;
  }
  newdeck(hand, 13);
  hand->obscure_until = 0;
  hand->direction = DECK_DIRECTION_DOWN;
  hand->window = place_deck(0, 0, 1, 1);
  hand->modified = true;

  newdeck_std(stock, false);
  shuffledeck(stock);
  stock->obscure_until = 52;
  stock->direction = DECK_DIRECTION_NONE;
  stock->window = place_deck(0, 1, 1, 1);
  stock->modified = true;

  newdeck(waste, 52);
  waste->obscure_until = 0;
  waste->direction = DECK_DIRECTION_NONE;
  waste->window = place_deck(0, 0, 1, 1);
  waste->modified = true;
}

void k_layout_table(deck_t *tableau, deck_t *stock) {
  for (int i = 0; i < 7; i++) {
    npop(stock, &(tableau[i]), i + 1);
  }
}


int k_tab_select(struct render_context *ctx, int deck, int card) {
  if (deck >= 7 || deck < 0 || card >= ctx->deck_data[deck].len || card < 0) {
    return 1;
  }
  
  ctx->deck_data[ctx->deck_select].modified = true;
  ctx->deck_data[deck].modified = true;
  
  ctx->deck_select = deck;
  ctx->card_select = card;
  return 0;
}

int k_deck_increment(struct render_context *ctx, bool cycle) {
  return k_tab_select(
      ctx, cycle ? (ctx->deck_select) + 1 % 7 : (ctx->deck_select) + 1,
			  ctx->card_select);
  
}

int k_deck_decrement(struct render_context *ctx, bool cycle) {
  return k_tab_select(
      ctx, cycle ? (ctx->deck_select) - 1 % ctx->decks : (ctx->deck_select) - 1,
			  ctx->card_select);
  
			  
}

int k_card_increment(struct render_context *ctx, bool cycle) {
  return k_tab_select(ctx, ctx->deck_select,
                          cycle ? (ctx->card_select) + 1 %
                                      ctx->deck_data[ctx->deck_select].len
                          : (ctx->card_select) + 1);
}

int k_card_decrement(struct render_context *ctx, bool cycle) {
    return k_tab_select(ctx, ctx->deck_select,
                          cycle ? (ctx->card_select) - 1 %
                                      ctx->deck_data[ctx->deck_select].len
                          : (ctx->card_select) - 1);
}



int klondike_tab_input(struct klondike_context *k_ctx, int ch) {
  switch (ch) {
  case MOVE_LEFT:
    return k_deck_decrement(&k_ctx->render_context, false);
    break;
  case MOVE_UP:
    return k_card_increment(&k_ctx->render_context, false);
    break;
  case MOVE_DOWN:
    return k_card_decrement(&k_ctx->render_context, false);
    break;
  case MOVE_RIGHT:
    return k_deck_increment(&k_ctx->render_context, false);
  default:
    return 1;
    break;
  }
}

int klondike_hand_input(struct klondike_context *k_ctx, int ch) { return 0; }


int klondike_input(struct render_context *ctx, int ch) {
  struct klondike_context *k_ctx = (struct klondike_context *)ctx;

  switch (ch) {
  case QUIT:
    ctx->signal = SIGNAL_QUIT;
    return 0;
    break;
  case REFRESH:
    ctx->signal = SIGNAL_RENDER_ALL;
    return 0;
    break;
  }

  if (k_ctx->state == KS_TABMODE) {
    return klondike_tab_input(k_ctx, ch);
  } else {
    return klondike_hand_input(k_ctx, ch);
  }
}


struct render_context *klondike_initialize() {
  struct klondike_context *ctx = malloc(sizeof(struct klondike_context));
  ctx->render_context.input_callback = &klondike_input;
  ctx->render_context.decks = 14;
  ctx->render_context.deck_data =
    malloc(sizeof(deck_t) * ctx->render_context.decks);

  ctx->render_context.deck_select = 0;
  ctx->render_context.card_select = 0;

  ctx->render_context.signal = SIGNAL_NONE;
  
  ctx->render_context.minheight = 4;
  ctx->render_context.minwidth = 7;

  ctx->tableau = ctx->render_context.deck_data;
  ctx->foundations = &(ctx->render_context.deck_data[7]);
  ctx->hand = &(ctx->render_context.deck_data[11]);
  ctx->stock = &(ctx->render_context.deck_data[12]);
  ctx->waste = &(ctx->render_context.deck_data[13]);

  k_deck_init(ctx->tableau, ctx->foundations, ctx->hand, ctx->stock,
              ctx->waste);
  k_layout_table(ctx->tableau, ctx->stock);

  ctx->state = KS_TABMODE;
  return (struct render_context *)ctx;
}
