#ifndef __DECK_H
#define __DECK_H

#include <curses.h>
#include <stdlib.h>

#include "card.h"
#include "config.h"

enum deck_direction {
  DECK_DIRECTION_NONE,
  DECK_DIRECTION_DOWN, 
  DECK_DIRECTION_RIGHT,
};

typedef struct {
  card_t *data;

  size_t maxlen;
  size_t len;
  size_t obscure_until;

  enum deck_direction direction;
  WINDOW *window;

  bool modified;
} deck_t;

void newdeck(deck_t *ptr, size_t maxlen);

void newdeck_std(deck_t *ptr, bool jokers);

void destroydeck(deck_t *deck);

int deck_getycoord(float y);
int deck_getxcoord(float x);

WINDOW *place_deck(float y, float x, float height, float width);

void shuffledeck(deck_t *deck);

int npop(deck_t *from, deck_t *to, size_t n);

int nflip(deck_t *from, deck_t *to, size_t n);

#endif // __DECK_H
