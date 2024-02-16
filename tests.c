#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "card.h"

int main(int argc, char **argv)
{
  /**************************************************
   * Name printing tests
   **************************************************/
  printf("Test: Name printing tests\n");
  
  char namebuf[3];

  /* Standard case (Nine of spades) */
  card_t card = newcard(9, SUIT_SPADE, CARD_NONE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " 9S") == 0);

  /* Ace should give A */
  card = newcard(1, SUIT_DIAMOND, CARD_NONE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " AD") == 0);

  /* 10 should have a leading 1 */
  card = newcard(10, SUIT_CLUB, CARD_NONE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, "10C") == 0);

  /* Normal joker shouldn't show suit */
  card = newcard(VAL_JOK1, SUIT_SPADE, CARD_NONE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " ??") == 0);

  /* 3rd joker should be different */
  card = newcard(VAL_JOK3, SUIT_SPADE, CARD_NONE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " !!") == 0);

  printf("All tests complete!\n");

  return 0;
}
