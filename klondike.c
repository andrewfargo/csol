#include "klondike.h"

#include "card.h"
#include "config.h"
#include "deck.h"

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

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
  hand->window = place_deck(0, 9, 13 * (2.0/CARD_HEIGHT), 1);
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


int k_tab_select(struct klondike_context *k_ctx, int deck, int card) {
  if (deck >= 7 || deck < 0 || card < 0) {
    return 1;
  }

  if (card >= k_ctx->tableau[deck].len) {
    card = k_ctx->tableau[deck].len - 1;
  }
  
  k_ctx->tableau[k_ctx->render_context.deck_select].modified = true;
  k_ctx->tableau[deck].modified = true;
  
  k_ctx->render_context.deck_select = deck;
  k_ctx->render_context.card_select = card;
  return 0;
}

int k_deck_increment(struct klondike_context *k_ctx) {
  int deck = k_ctx->render_context.deck_select + 1;
  int card = k_ctx->render_context.card_select;
  
  return k_tab_select(k_ctx, deck, card);  
}

int k_deck_decrement(struct klondike_context *k_ctx) {
  int deck = k_ctx->render_context.deck_select - 1;
  int card = k_ctx->render_context.card_select;
  
  return k_tab_select(k_ctx, deck, card); 
}

int k_card_increment(struct klondike_context *k_ctx) {
  int deck = k_ctx->render_context.deck_select;
  int card = k_ctx->render_context.card_select + 1;
  
  return k_tab_select(k_ctx, deck, card);
}

int k_card_decrement(struct klondike_context *k_ctx) {
  int deck = k_ctx->render_context.deck_select;
  int card = k_ctx->render_context.card_select - 1;
  
  return k_tab_select(k_ctx, deck, card);
}
  
void k_hand_move(struct klondike_context *k_ctx, int deck) {
  k_ctx->tableau[k_ctx->hand_position].modified = true;
  mvwin(k_ctx->hand->window, CARD_HEIGHT + (2 * (k_ctx->tableau[deck].len + 1)) + 2,
        deck_getxcoord(deck));
  k_ctx->hand_position = deck;
  k_ctx->tableau[k_ctx->hand_position].modified = true;
  k_ctx->hand->modified = true;
}

int k_hand_decrement(struct klondike_context *k_ctx) {
  if (k_ctx->hand_position <= 0)
    return 1;
  k_hand_move(k_ctx, k_ctx->hand_position - 1);
  return 0;
}

int k_hand_increment(struct klondike_context *k_ctx) {
  if (k_ctx->hand_position >= 6)
    return 1;
  k_hand_move(k_ctx, k_ctx->hand_position + 1);
  return 0;
}

void k_update_tableau(struct klondike_context *k_ctx) {
  for (int i = 0; i < 7; i++) {
    k_ctx->tableau[i].obscure_until =
      MIN(k_ctx->tableau[i].obscure_until, k_ctx->tableau[i].len - 1);
    k_ctx->tableau[i].modified = true;
  }
}

int k_tab_pickup(struct klondike_context *k_ctx) {
  const int deck = k_ctx->render_context.deck_select;
  const int card = k_ctx->render_context.card_select;

  if (card < k_ctx->tableau[deck].obscure_until) {
    return 1;
  }

  k_ctx->render_context.deck_select = -1;
  
  npop(&k_ctx->tableau[deck], k_ctx->hand, k_ctx->tableau[deck].len - card);
  k_hand_move(k_ctx, deck);
  
  k_ctx->state = KS_HAND_TAB;
  k_ctx->taken_position = deck;
  
  return 0;
}

int k_hand_putdown(struct klondike_context *k_ctx) {
  /* These bool definitions check placement validity */
  const deck_t tgt_deck = k_ctx->tableau[k_ctx->hand_position];
  const bool replace = k_ctx->state == KS_HAND_TAB &&
                 k_ctx->hand_position == k_ctx->taken_position;
  const bool empty = tgt_deck.len <= 0;
  const bool king = k_ctx->hand->data[0].value == VAL_KING;
  const bool ordered =
    k_ctx->hand->data[0].value == tgt_deck.data[tgt_deck.len - 1].value - 1;
  const bool opp_suit = is_red(k_ctx->hand->data[0].suit) !=
                  is_red(tgt_deck.data[tgt_deck.len - 1].suit);

  const bool valid = (empty && king) || replace || (!empty && opp_suit && ordered);
  if (!valid) {
    return 1;
  }



  k_ctx->render_context.deck_select = k_ctx->hand_position;
  k_ctx->render_context.card_select = k_ctx->tableau[k_ctx->hand_position].len;
  
  npop(k_ctx->hand, &k_ctx->tableau[k_ctx->hand_position], k_ctx->hand->len);
  k_ctx->hand->modified = true;
  k_ctx->tableau[k_ctx->hand_position].modified = true;
  k_ctx->state = KS_TABMODE;
  mvwin(k_ctx->hand->window, 0, CARD_WIDTH * 15);

  k_update_tableau(k_ctx);

  return 0;
}

int k_take_foundation(struct klondike_context *k_ctx, int foundation) {
  deck_t *foundation_p = &k_ctx->foundations[foundation];
  if (foundation_p->len == 0) {
    return 1;
  }
  
  npop(foundation_p, k_ctx->hand, 1);
  k_hand_move(k_ctx, k_ctx->render_context.deck_select);

  k_ctx->render_context.deck_select = -1;
  
  k_ctx->state = KS_HAND_FOUNDATION;
  foundation_p->modified = true;
  
  return 0;
}

int k_tab_foundation(struct klondike_context *k_ctx) {
  const int foundation = k_ctx->tableau[k_ctx->render_context.deck_select]
                             .data[k_ctx->render_context.card_select]
                             .suit;
  const bool right_card =
      k_ctx->render_context.card_select ==
      k_ctx->tableau[k_ctx->render_context.deck_select].len - 1;
  const bool ace_on_empty = k_ctx->tableau[k_ctx->render_context.deck_select]
                                    .data[k_ctx->render_context.card_select]
                                    .value == 1 &&
                            k_ctx->foundations[foundation].len == 0;
  const bool right_value =
      k_ctx->tableau[k_ctx->render_context.deck_select]
          .data[k_ctx->render_context.card_select]
          .value == k_ctx->foundations[foundation]
                            .data[k_ctx->foundations[foundation].len - 1]
                            .value +
                    1;

  const bool valid = right_card && (right_value || ace_on_empty);
  if (!valid) {
    return 1;
  }

  k_ctx->render_context.card_select--;

  npop(&k_ctx->tableau[k_ctx->render_context.deck_select],
       &k_ctx->foundations[foundation], 1);
  k_ctx->tableau[k_ctx->render_context.deck_select].modified = true;
  k_ctx->foundations[foundation].modified = true;

  k_update_tableau(k_ctx);
  
  return 0;
  
}

int k_hand_foundation(struct klondike_context *k_ctx) {
  const int foundation = k_ctx->hand->data[0].suit;
  const bool right_card = k_ctx->hand->len == 1;
  const bool ace_on_empty = k_ctx->hand->data[0].value == 1 &&
                            k_ctx->foundations[foundation].len == 0;
  const bool right_value = k_ctx->hand->data[0].value ==
                           k_ctx->foundations[foundation]
                                   .data[k_ctx->foundations[foundation].len - 1]
                                   .value +
                           1;
  const bool valid = right_card && (right_value || ace_on_empty);
  if (!valid) {
    return 1;
  }

  k_ctx->render_context.deck_select = k_ctx->hand_position;
  k_ctx->render_context.card_select =
    k_ctx->tableau[k_ctx->hand_position].len - 1;

  npop(k_ctx->hand, &k_ctx->foundations[foundation], 1);
  k_ctx->hand->modified = true;
  k_ctx->foundations[foundation].modified = true;
  k_ctx->state = KS_TABMODE;
  mvwin(k_ctx->hand->window, 0, CARD_WIDTH * 15);

  k_update_tableau(k_ctx);
  return 0;
}


int k_take_waste(struct klondike_context *k_ctx) {
  if (k_ctx->waste->len == 0) {
    return 1;
  }

  npop(k_ctx->waste, k_ctx->hand, 1);
  k_hand_move(k_ctx, k_ctx->render_context.deck_select);

  k_ctx->state = KS_HAND_WASTE;
  k_ctx->waste->modified = true;
  return 0;
}


int k_put_waste(struct klondike_context *k_ctx) {
  if (k_ctx->state != KS_HAND_WASTE) {
    return 1;
  }

  k_ctx->render_context.deck_select = k_ctx->hand_position;
  k_ctx->render_context.card_select = k_ctx->tableau[k_ctx->hand_position].len - 1;

  npop(k_ctx->hand, k_ctx->waste, 1);
  
  k_ctx->waste->modified = true;
  k_ctx->hand->modified = true;
  k_ctx->tableau[k_ctx->hand_position].modified = true;
  k_ctx->state = KS_TABMODE;

  mvwin(k_ctx->hand->window, 0, CARD_WIDTH * 15);

  k_update_tableau(k_ctx);
  return 0;
}

int k_stock_advance(struct klondike_context *k_ctx) {
  if (k_ctx->stock->len == 0 && k_ctx->refreshes_left == 0) {
    return 1;
  }

  if (k_ctx->stock->len == 0) {
    nflip(k_ctx->waste, k_ctx->stock, k_ctx->waste->len);
    k_ctx->refreshes_left--;
  } else {
    nflip(k_ctx->stock, k_ctx->waste, 1);
  }

  k_ctx->waste->modified = true;
  k_ctx->stock->modified = true;
  
  return 0;
}

int klondike_tab_input(struct klondike_context *k_ctx, int ch) {
  switch (ch) {
  case MOVE_LEFT:
    return k_deck_decrement(k_ctx);
    break;
  case MOVE_DOWN:
    return k_card_increment(k_ctx);
    break;
  case MOVE_UP:
    return k_card_decrement(k_ctx);
    break;
  case MOVE_RIGHT:
    return k_deck_increment(k_ctx);
  case PRIMARY:
    return k_tab_pickup(k_ctx);
    break;
  case MOVE_FOUNDATION:
    return k_tab_foundation(k_ctx);
    break;
  case TAKE_FOUNDATION0:
    return k_take_foundation(k_ctx, 0);
    break;
  case TAKE_FOUNDATION1:
    return k_take_foundation(k_ctx, 1);
    break;
  case TAKE_FOUNDATION2:
    return k_take_foundation(k_ctx, 2);
    break;
  case TAKE_FOUNDATION3:
    return k_take_foundation(k_ctx, 3);
    break;
  case STOCK_ADVANCE:
    return k_stock_advance(k_ctx);
    break;
  case MOVE_WASTE:
    return k_take_waste(k_ctx);
    break;
  default:
    return 1;
    break;
  }
}

int klondike_hand_input(struct klondike_context *k_ctx, int ch) {
  switch (ch) {
  case MOVE_LEFT:
    return k_hand_decrement(k_ctx);
    break;
  case MOVE_RIGHT:
    return k_hand_increment(k_ctx);
    break;
  case PRIMARY:
    return k_hand_putdown(k_ctx);
    break;
  case MOVE_WASTE:
    return k_put_waste(k_ctx);
    break;
  case MOVE_FOUNDATION:
    return k_hand_foundation(k_ctx);
    break;
  default:
    return 1;
    break;
  }
}

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
  ctx->hand_position = 0;
  ctx->taken_position = 0;

  ctx->refreshes_left = 1;
  
  return (struct render_context *)ctx;
}
