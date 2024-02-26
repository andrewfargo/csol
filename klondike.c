#include "config.h"
#include "klondike.h"
#include "render.h"
#include "deck.h"
#include "card.h"

void
k_deck_init(deck_t *tableau, deck_t *foundations, deck_t *hand,
	    deck_t *stock, deck_t *waste)
{
  for (int i = 0; i < 7; i++)
    {
      newdeck(&(tableau[i]), 13);
      tableau[i].obscure_until = i;
      tableau[i].direction = DECK_DIRECTION_DOWN;
      tableau[i].window = place_deck(1, i, 20 * (2.0 / CARD_HEIGHT), 1);
      tableau[i].modified = true;
    }
  for (int i = 0; i < 4; i++)
    {
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

void
k_layout_table(deck_t *tableau, deck_t *stock)
{
  for (int i = 0; i < 7; i++)
    {
      npop(stock, &(tableau[i]), 7 - i);
    }
}

int
klondike_input(struct render_context *ctx, int ch)
{
  return 0;
}

struct render_context *
klondike_initialize()
{
  struct klondike_context *ctx = malloc(sizeof(struct klondike_context));
  ctx->render_context.input_callback = &klondike_input;
  ctx->render_context.decks = 14;
  ctx->render_context.deck_data = malloc(sizeof(deck_t) * ctx->render_context.decks);
  ctx->render_context.minheight = 4;
  ctx->render_context.minwidth = 7;

  ctx->tableau = ctx->render_context.deck_data;
  ctx->foundations = &(ctx->render_context.deck_data[7]);
  ctx->hand = &(ctx->render_context.deck_data[11]);
  ctx->stock = &(ctx->render_context.deck_data[12]);
  ctx->waste = &(ctx->render_context.deck_data[13]);

  k_deck_init(ctx->tableau, ctx->foundations, ctx->hand,
	      ctx->stock, ctx->waste);
  k_layout_table(ctx->tableau, ctx->stock);

  ctx->state = KS_TABMODE;
  return (struct render_context *) ctx;
}
