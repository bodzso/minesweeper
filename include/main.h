#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>

typedef struct{
    int **arr;
    int rown;
    int coln;
    int minen;
    int limit;
    bool lose;
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

void init();
void main_graphics();
void start();
int choice();
int custom();
void load();
extern board b;
extern SDL_Surface *tileset;
extern SDL_Surface *screen;
