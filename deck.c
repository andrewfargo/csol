#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "deck.h"
#include "card.h"
#include "config.h"

void newdeck(deck_t *ptr, size_t maxlen) {
  ptr->maxlen = maxlen;
  ptr->data = malloc(sizeof(card_t) * maxlen);
  ptr->len = 0;
  ptr->obscure_until = 0;
  ptr->direction = DECK_DIRECTION_NONE;
  ptr->window = NULL;
  ptr->modified = false;
}

void newdeck_std(deck_t *ptr, bool jokers) {
  size_t maxlen = jokers ? 54 : 52;
  newdeck(ptr, maxlen);
  for (int suit = SUIT_HEART; suit <= SUIT_SPADE; suit++) {
    for (int value = 1; value <= VAL_KING; value++) {
      ptr->data[vs_to_index(value, suit)] = newcard(value, suit);
    }
  }
  if (jokers) {
    ptr->data[52] = newcard(VAL_JOK1, SUIT_HEART);
    ptr->data[53] = newcard(VAL_JOK2, SUIT_CLUB);
  }
  ptr->len = maxlen;
}

WINDOW *place_deck(float y, float x, float height, float width) {
  return newwin((int)(height * CARD_HEIGHT), (int)(width * CARD_WIDTH),
                (int)(y * (CARD_HEIGHT + 1)) + 1, (int)(x * (CARD_WIDTH + 1)) + 1);
}

void data_swap_indices(card_t *data, size_t i, size_t j) {
  card_t temp = data[j];
  data[j] = data[i];
  data[i] = temp;
}

void shuffledeck(deck_t *deck) {
  int j;
  if (deck->len < 2)
    return;
  for (int i = 0; i < deck->len - 2; i++) {
    j = (rand() % (deck->len - i)) + i;
    data_swap_indices(deck->data, i, j);
  }
}

int npop(deck_t *from, deck_t *to, size_t n) {
  if (from->len < n || (to->maxlen - to->len) < n)
    return 1;
  for (int i = 0; i < n; i++) {
    size_t from_idx = from->len - n + i;
    size_t to_idx = to->len + i;
    to->data[to_idx] = from->data[from_idx];
  }
  from->len -= n;
  to->len += n;
  return 0;
}

void destroydeck(deck_t *deck) {
  free(deck->data);
  if (deck->window != NULL)
    delwin(deck->window);
  free(deck);
}
