#include <stdio.h>
#include <string.h>
#include "card.h"

/**
 * Suit conversion array for easy printing.
 */
const char *suit_conv = "SCDH";

/**
 * Value conversion array for easy printing.
 * Note for 10, the 1 is handled in-implementation.
 */
const char *val_conv = "!A234567890JQK??";



card_t newcard(int value, int suit, int state)
{
  card_t card = {.value = value, .suit = suit, .state = state};
  return card;
}

int sprintname(char *buf, card_t card)
{
  /* Handle the 10 case */
  if (card.value == 10)
    buf[0] = '1';
  else
    buf[0] = ' ';
  
  buf[1] = val_conv[card.value];

  /* If it's a joker, we don't want the suit. */
  switch (card.value)
    {
  case VAL_JOK1:
  case VAL_JOK2:
    buf[2] = '?';
    break;
  case VAL_JOK3:
    buf[2] = '!';
    break;
  default:
    buf[2] = suit_conv[card.suit];
    break;
  }
  
  return 0;
}

