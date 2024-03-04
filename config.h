#ifndef __CONFIG_H
#define __CONFIG_H

#define CARD_WIDTH 10
#define CARD_HEIGHT 8

#include <stdint.h>
#include <ncurses.h>

/* COLORS */

#define WINDOW_BG COLOR_GREEN

#define BACK_FG COLOR_WHITE
#define BACK_BG COLOR_BLUE

#define RED_FG COLOR_RED
#define RED_BG COLOR_WHITE

#define BLACK_FG COLOR_BLACK
#define BLACK_BG COLOR_WHITE

#define STD_FG COLOR_BLACK
#define STD_BG COLOR_WHITE

#define SEL_FG COLOR_BLUE
#define SEL_BG COLOR_YELLOW

#define BACK_CHAR ACS_CKBOARD


/* CONTROLS */

#define MOVE_UP 'k'
#define MOVE_DOWN 'j'
#define MOVE_LEFT 'h'
#define MOVE_RIGHT 'l'
#define QUIT 'q'
#define REFRESH 'g'
#define PRIMARY KEY_SPACE

/* game specific controls can be found in the [gamemode].h file */


#define PIP_CHAR '@'

/* The following definitions encode pip locations as bits according
 * to the following chart:
 *  l m r
 * t. . .
 * i. . .
 * m. . .
 * j. . .
 * b. . .
 *
 */
static const uint64_t pip_tl = 1ULL << (4 * 8 + 6);
static const uint64_t pip_tm = 1ULL << (4 * 8 + 4);
static const uint64_t pip_tr = 1ULL << (4 * 8 + 2);

static const uint64_t pip_il = 1ULL << (3 * 8 + 6);
static const uint64_t pip_im = 1ULL << (3 * 8 + 4);
static const uint64_t pip_ir = 1ULL << (3 * 8 + 2);

static const uint64_t pip_ml = 1ULL << (2 * 8 + 6);
static const uint64_t pip_mm = 1ULL << (2 * 8 + 4);
static const uint64_t pip_mr = 1ULL << (2 * 8 + 2);

static const uint64_t pip_jr = 1ULL << (1 * 8 + 6);
static const uint64_t pip_jm = 1ULL << (1 * 8 + 4);
static const uint64_t pip_jl = 1ULL << (1 * 8 + 2);

static const uint64_t pip_bl = 1ULL << 6;
static const uint64_t pip_bm = 1ULL << 4;
static const uint64_t pip_br = 1ULL << 2;

/* I'm not particularly proud of this, but it allows
   customizable pip structures. */
static const uint64_t pip_bits[16] = {
    0x0, pip_mm, pip_tm | pip_bm, pip_tm | pip_mm | pip_bm,
    pip_tl | pip_tr | pip_bl | pip_br,
    pip_tl | pip_tr | pip_bl | pip_br | pip_mm,
    pip_tl | pip_tr | pip_ml | pip_mr | pip_bl | pip_br,
    pip_tl | pip_tr | pip_ml | pip_mr | pip_jm | pip_bl | pip_br,
    pip_tl | pip_tr | pip_im | pip_ml | pip_mr | pip_jm | pip_bl | pip_br,
    pip_tl | pip_tr | pip_il | pip_ir | pip_ml | pip_mr | pip_jm | pip_bl |
        pip_br,
    pip_tl | pip_tr | pip_il | pip_im | pip_ir | pip_ml | pip_mr | pip_jm |
        pip_bl | pip_br,
    0x00, 0x00, 0x00, 0x00,
    0x00};



#endif // __CONFIG_H
