#include <stdio.h>
#include <stdlib.h>

#include "deck.h"
#include "card.h"

deck_t*
newdeck(size_t maxlen)
{
  deck_t *ret = malloc(sizeof(deck_t));
  ret->maxlen = maxlen;
  ret->data = malloc(sizeof(card_t) * maxlen);
  ret->len = 0;
  ret->obscure_until = 0;
  return ret;
}

deck_t*
newdeck_std(bool jokers)
{
  size_t maxlen = jokers ? 54 : 52;
  deck_t *ret = newdeck(maxlen);
  for(int suit = SUIT_HEART; suit <= SUIT_SPADE; suit++)
    {
      for(int value = 1; value <= VAL_KING; value++)
	{
	  ret->data[vs_to_index(value, suit)] = newcard(value, suit);
	}
    }
  if (jokers)
    {
      ret->data[52] = newcard(VAL_JOK1, SUIT_HEART);
      ret->data[53] = newcard(VAL_JOK2, SUIT_CLUB);
    }
  ret->len = maxlen;
  return ret;
}

void
data_swap_indices(card_t *data, size_t i, size_t j)
{
  card_t temp = data[j];
  data[j] = data[i];
  data[i] = temp;
}

void
shuffledeck(deck_t *deck)
{
  int j;
  if (deck->len < 2)
    return;
  for(int i = 0; i < deck->len - 2; i++)
    {
      j = (rand() % (deck->len - i)) + i;
      data_swap_indices(deck->data, i, j);
    }
}

int
npop(deck_t *from, deck_t *to, size_t n)
{
  if (from->len < n || (to->maxlen - to->len) < n)
    return 1;
  for (int i = 0; i < n; i++)
    {
      size_t from_idx = from->len - n + i;
      size_t to_idx = to->len + i;
      to->data[to_idx] = from->data[from_idx];
    }
  from->len -= n;
  to->len += n;
  return 0;
}
  
void
destroydeck(deck_t *deck)
{
  free(deck->data);
  free(deck);
}

void
debug_printdeck(deck_t *deck)
{
  char buf[3];
  printf("[ ");
  for (int i = 0; i < deck->len; i++)
    {
      sprintname(buf, deck->data[i]);
      printf(buf);
      printf(" ");
    }
  printf("]\n");
}
