#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

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

void init();
void init_graphics();
void start();
int choice();
void load();
extern board b;

#endif // MAIN_H_INCLUDED
