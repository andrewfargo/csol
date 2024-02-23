#ifndef __RENDER_H
#define __RENDER_H

#include "deck.h"

struct render_context {
  int (*input_callback)(int ch);
  deck_t **render_stack;
  int render_stack_len;
};

#endif //__RENDER_H
