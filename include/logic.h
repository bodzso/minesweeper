#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

typedef struct{
    int **arr;
    int rown;
    int coln;
    int minen;
    int limit;
    bool loose;
    bool win;
    bool hard;
    bool generated;
    bool loaded;
    int mine;
    int mflagged;
    int smiley;
    int counter;
    int revealed;
}board;

board b = {
    .arr = NULL,
    .rown = 9, .coln = 9, .minen = 5, .limit = 0,
    .loose = false, .win = false, .hard = false,
    .generated = false, .loaded = false,
    .mine = 0, .mflagged = 0,
    .smiley = 0,
    .counter = 0,
    .revealed = 0
};

void paint();
Uint32 draw_time(Uint32 ms, void *param);
extern SDL_Surface *tileset;
extern SDL_Surface *screen;

#define FSIZ 16 //field size
#define BSIZ 10 //border size

#endif // LOGIC_H_INCLUDED
