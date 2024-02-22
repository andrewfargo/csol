#include <stdio.h>
#include <string.h>
#include "card.h"

/* Print formatters */
const char *suit_conv = "HCDS";

const char *val_conv = "!A234567890JQK??";

card_t
newcard(int value, int suit)
{
  card_t card = {.value = value, .suit = suit};
  return card;
}

void
sprintname(char *buf, card_t card)
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
}
