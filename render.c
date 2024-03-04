#include "render.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "config.h"
#include "card.h"
#include "deck.h"
#include "klondike.h"

/* Print formatters */
const char *suit_conv = "HCDS";

const char *val_conv = "!A234567890JQK??";

/* Color instantiations, defs in config.h */
#define BACK_ATTR (COLOR_PAIR(CPAIR_BACK))
#define RED_ATTR (A_BOLD | COLOR_PAIR(CPAIR_RED))
#define BLACK_ATTR (COLOR_PAIR(CPAIR_BLACK))
#define STD_ATTR (COLOR_PAIR(CPAIR_STD))
#define SELECT_ATTR (COLOR_PAIR(CPAIR_SELECT) | A_BOLD)
#define WINDOW_ATTR (COLOR_PAIR(CPAIR_WINDOW))

enum color_pairs {
  CPAIR_BACK = 1,
  CPAIR_RED,
  CPAIR_BLACK,
  CPAIR_STD,
  CPAIR_SELECT,
  CPAIR_WINDOW
};


void init_card_colors() {
  init_pair(CPAIR_BACK, BACK_FG, BACK_BG);
  init_pair(CPAIR_RED, RED_FG, RED_BG);
  init_pair(CPAIR_BLACK, BLACK_FG, BLACK_BG);
  init_pair(CPAIR_STD, STD_FG, STD_BG);
  init_pair(CPAIR_SELECT, SEL_FG, SEL_BG);
  init_pair(CPAIR_WINDOW, WINDOW_BG, WINDOW_BG);
}

/* Rendering helpers */

void sprintpips(char *buf, card_t card) {
  for (int i = 39; i >= 0; i--) {
    buf[i] = ((1ULL << i) & (pip_bits[card.value])) ? PIP_CHAR : ' ';
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

void render_top_at_curs(WINDOW *win) {
  waddch(win, ACS_ULCORNER);
  for (int i = 0; i < 8; i++)
    waddch(win, ACS_HLINE);
  waddch(win, ACS_URCORNER);
}

void render_bottom_at_curs(WINDOW *win) {
  waddch(win, ACS_LLCORNER);
  for (int i = 0; i < 8; i++)
    waddch(win, ACS_HLINE);
  waddch(win, ACS_LRCORNER);
}

void render_sides(WINDOW *win, int yoff) {
    for (int i = 0; i < 6; i++) {
      wmove(win, yoff + i + 1, 0);
      waddch(win, ACS_VLINE);
      wmove(win, yoff + i + 1, 9);
      waddch(win, ACS_VLINE);
    }
}

void render_frame(WINDOW *win, int yoff, bool obscure, bool select) {

  if (select) {
    wattron(win, SELECT_ATTR);
  } else {
    wattron(win, obscure ? BACK_ATTR : STD_ATTR);
  }
  
  wmove(win, yoff, 0);
  render_top_at_curs(win);
  render_sides(win, yoff);
  render_bottom_at_curs(win);
  wattroff(win, SELECT_ATTR | STD_ATTR | BACK_ATTR);
}


void render_pips(WINDOW *win, card_t card, int yoff) {
  char *pipstr = malloc(sizeof(char) * 64);
  sprintpips(pipstr, card);
  
  for (int i = 0; i < 5; i++) {
    wmove(win, yoff + 2 + i, 1);
    waddnstr(win, pipstr, 8);
    pipstr += 8;
  }
}

/* pips are expensive, only render whats necessary (topcard) */
void render_card(WINDOW *win, card_t card, int yoff, bool topcard, bool select) {
  render_frame(win, yoff, false, select);

  if (select) {
    wattron(win, SELECT_ATTR);
  } else if (is_red(card.suit)) {
    wattron(win, RED_ATTR);
  } else {
    wattron(win, BLACK_ATTR);
  }
  
  char name[4];
  sprintname(name, card);
  wmove(win, yoff + 1, 1);
  wprintw(win, "%s     ", name);

  if (!topcard) {
    wattroff(win, SELECT_ATTR | RED_ATTR | BLACK_ATTR);
    return;
  }
  
  render_pips(win, card, yoff);

  wattroff(win, SELECT_ATTR | RED_ATTR | BLACK_ATTR);
}

void render_obscure(WINDOW *win, int yoff, bool select) {
  render_frame(win, yoff, true, select);

  if (select) {
    wattron(win, SELECT_ATTR);
  } else {
    wattron(win, BACK_ATTR);
  }

  for (int i = 0; i < 6; i++) {
    wmove(win, yoff + i + 1, 1);
    for (int j = 0; j < 8; j++) {
      waddch(win, BACK_CHAR);
    }
  }
  wattroff(win, SELECT_ATTR | BACK_ATTR);
}

void render_direction_none(deck_t *deck, bool select) {
  if (deck->len == 0) {
    return;
  } else if (deck->len >= deck->obscure_until) {
    render_card(deck->window, deck->data[deck->len - 1], 0, true, select);
  } else {
    render_obscure(deck->window, 0, select);
  }
}

void render_direction_down(deck_t *deck, bool deck_select, int card_select) {
  for (int i = 0; i < deck->len; i++)
    {
    bool select = deck_select && (card_select == i);
    if (i >= deck->obscure_until) {
      render_card(deck->window, deck->data[i], 2 * i, (i == deck->len - 1), select);
    } else {
      render_obscure(deck->window, 2 * i, select);
    }
    }
};

void render(struct render_context *ctx) {
  for (int i = 0; i < ctx->decks; i++) {
    if (!(ctx->deck_data[i].modified || ctx->signal == SIGNAL_RENDER_ALL)) {
      continue;
    }

    werase(ctx->deck_data[i].window);
    wbkgd(ctx->deck_data[i].window, WINDOW_ATTR);
    
    switch (ctx->deck_data[i].direction) {
    case DECK_DIRECTION_NONE:
      render_direction_none(&ctx->deck_data[i], ctx->deck_select == i);
      break;
    case DECK_DIRECTION_DOWN:
      render_direction_down(&ctx->deck_data[i], ctx->deck_select == i,
                            ctx->card_select);
      break;
    default:
      exit(1); //not implemented
    }
    wrefresh(ctx->deck_data[i].window);

    ctx->deck_data[i].modified = false;
  }
}


int main(int argc, char **argv) {
  srand(time(NULL));
  initscr();
  noecho();
  cbreak();

  start_color();
  init_card_colors();
  bkgd(WINDOW_ATTR);
  refresh();

  struct render_context *ctx = klondike_initialize();

  while (ctx->signal != SIGNAL_QUIT) {
    ctx->signal = SIGNAL_NONE;
    render(ctx);

    int code = ctx->input_callback(ctx, getch());
    
    if (code && FLASH_ON_ERROR) {
      flash();
    }
  }
  
  endwin();
  return 0;
}
