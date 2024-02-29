#include "render.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "config.h"
#include "deck.h"
#include "klondike.h"

/* Print formatters */
const char *suit_conv = "HCDS";

const char *val_conv = "!A234567890JQK??";

void sprintpips(char *buf, card_t card) {
  for (int i = 39; i >= 0; i--) {
    buf[i] = ((1ULL << i) & (pip_bits[card.value])) ? pip : ' ';
  }
  buf[40] = '\0';
}

void sprintname(char *buf, card_t card) {
  /* Handle the 10 case */
  if (card.value == 10)
    buf[0] = '1';
  else
    buf[0] = ' ';

  buf[1] = val_conv[card.value];

  /* If it's a joker, we don't want the suit. */
  switch (card.value) {
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
  buf[3] = '\0';
}

void render_card(WINDOW *win, card_t card, int yoff, bool topcard) {
  wmove(win, yoff, 0);
  waddstr(win, "/--------\\");
  char name[4];
  sprintname(name, card);
  wprintw(win, "|%s     |", name);
  if (!topcard) {
    return;
  }

  char *pipstr = malloc(sizeof(char) * 64);
  sprintpips(pipstr, card);
  for (int i = 0; i < 5; i++) {
    wmove(win, yoff + 2 + i, 0);
    waddch(win, '|');
    waddnstr(win, pipstr, 8);
    waddch(win, '|');
    pipstr += 8;
  }
  waddstr(win, "\\________/");
}

void render_obscure(WINDOW *win, int yoff) {
  wmove(win, yoff, 0);
  waddstr(win, "/--------\\");
  waddstr(win, card_back);
  waddstr(win, "\\________/");
  
}

void render_direction_none(deck_t *deck) {
  if (deck->len == 0) {
    return;
  } else if (deck->len >= deck->obscure_until) {
    render_card(deck->window, deck->data[deck->len - 1], 0, true);
  } else {
    render_obscure(deck->window, 0);
  }
}

void render_direction_down(deck_t *deck) {
  for (int i = 0; i < deck->len; i++)
    if (i >= deck->obscure_until) {
      render_card(deck->window, deck->data[i], 2 * i, (i == deck->len - 1));
    } else {
      render_obscure(deck->window, 2 * i);
    }
};

void render(deck_t *deck_data, int decks) {
  for (int i = 0; i < decks; i++) {
    if (deck_data[i].modified) {
      switch (deck_data[i].direction) {
      case DECK_DIRECTION_NONE:
        render_direction_none(&deck_data[i]);
        break;
      case DECK_DIRECTION_DOWN:
        render_direction_down(&deck_data[i]);
        break;
      default:
	exit(1); //not implemented
      }
      wrefresh(deck_data[i].window);
    }
  }
}

int main(int argc, char **argv) {
  srand(time(NULL));
  initscr();
  cbreak();
  refresh();
  struct render_context *ctx = klondike_initialize();
  render(ctx->deck_data, ctx->decks);
  getch();
  endwin();
  return 0;
}
