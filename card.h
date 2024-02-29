#ifndef __CARD_H
#define __CARD_H

#define vs_to_index(value, suit) ((suit)*13 + (value)-1)

#define is_red(suit) ((suit) == SUIT_HEART || (suit) == SUIT_DIAMOND)
#define is_black(suit) ((suit) == SUIT_CLUB || (suit) == SUIT_SPADE)

enum suit {
  SUIT_HEART,
  SUIT_CLUB, 
  SUIT_DIAMOND,
  SUIT_SPADE,  
};

enum special_values {
  VAL_JOK3 = 0,
  VAL_JACK = 11,
  VAL_QUEEN = 12,
  VAL_KING = 13,
  VAL_JOK1 = 14,
  VAL_JOK2 = 15 
};

typedef struct {
  enum suit suit : 2;
  unsigned int value : 4;
} card_t;

card_t newcard(int value, int suit);

#endif // __CARD_H
