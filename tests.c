#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "card.h"
#include "deck.h"

int main(int argc, char **argv)
{
  /**************************************************
   * Name printing tests                            *
   **************************************************/
  printf("Test: Name printing tests\n");
  
  char namebuf[3];

  /* Standard case (Nine of spades) */
  card_t card = newcard(9, SUIT_SPADE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " 9S") == 0);

  /* Ace should give A */
  card = newcard(1, SUIT_DIAMOND);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " AD") == 0);

  /* 10 should have a leading 1 */
  card = newcard(10, SUIT_CLUB);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, "10C") == 0);

  /* Normal joker shouldn't show suit */
  card = newcard(VAL_JOK1, SUIT_SPADE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " ??") == 0);

  /* 3rd joker should be different */
  card = newcard(VAL_JOK3, SUIT_SPADE);
  sprintname(namebuf, card);
  assert(strcmp(namebuf, " !!") == 0);


  /**************************************************
   * Deck tests                                     *
   **************************************************/
  printf("Test: Deck tests\n");
  
  /* newdeck returns relatively proper decks */
  deck_t *test_deck = newdeck(0);
  assert(test_deck != NULL);
  assert(test_deck->data != NULL);
  destroydeck(test_deck);

  test_deck = newdeck(1);
  assert(test_deck != NULL);
  /* - try accessing the element */
  card_t junk_card = test_deck->data[0];
  destroydeck(test_deck);

  test_deck = newdeck(54);
  assert(test_deck != NULL);
  assert(test_deck->maxlen == 54);
  junk_card = test_deck->data[53];
  destroydeck(test_deck);

  /* newdeck_std returns appropriately sized deck */
  test_deck = newdeck_std(false);
  assert(test_deck->maxlen == 52);
  destroydeck(test_deck);

  test_deck = newdeck_std(true);
  assert(test_deck->maxlen == 54);
  destroydeck(test_deck);

  /* npop works as intended */
  test_deck = newdeck_std(false);
  deck_t *destination = newdeck(10);

  /* - obviously incorrect values fail */
  assert(npop(test_deck, destination, 50) == 1);
  assert(npop(destination, test_deck, 50) == 1);

  /* - obviously correct values work */
  assert(npop(test_deck, destination, 0) == 0);

  assert(npop(test_deck, destination, 5) == 0);

  assert(test_deck->len == 52 - 5);
  assert(destination->len == 5);

  assert(npop(destination, test_deck, 5) == 0);

  /* - subtly correct values work */
  assert(npop(test_deck, destination, 10) == 0);
  assert(npop(destination, test_deck, 10) == 0);

  /* - and subtly incorrect values fail */
  assert(npop(test_deck, destination, 11) == 1);

  destroydeck(test_deck);
  destroydeck(destination);

  printf("All tests complete!\n");

  
  return 0;
}
