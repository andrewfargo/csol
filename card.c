#include "card.h"

#include <stdio.h>
#include <string.h>


card_t newcard(int value, int suit) {
  card_t card = {.value = value, .suit = suit};
  return card;
}

