#include <ncurses.h>
#include <stdio.h>
#include "render.h"
#include "klondike.h"

void
render(deck_t *deck_data, int decks)
{
  for (int i = 0; i < decks; i++)
    {
      if (deck_data[i].modified)
	{
          box(deck_data[i].window, 0, 0);
	  wrefresh(deck_data[i].window);
	}
    }
}

int main(int argc, char **argv) {
  initscr();
  cbreak();
  refresh();
  struct render_context *ctx = klondike_initialize();
  render(ctx->deck_data, ctx->decks);
  getch();
  endwin();

  printf("%d\n", ctx->decks);
  return 0;
}
