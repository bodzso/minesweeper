#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>

#define FSIZ 16 //field size
#define BSIZ 10 //border size

//SDL
SDL_Surface *screen;
SDL_Surface *tileset;

//tileset clipping
SDL_Rect clip[] = {
//   x  y  sx  sy
    {0, 0, 16, 16},  //0 empty
    {16, 0, 16, 16}, //1
    {32, 0, 16, 16},
    {48, 0, 16, 16},
    {64, 0, 16, 16},
    {80, 0, 16, 16},
    {96, 0, 16, 16},
    {112, 0, 16, 16},
    {128, 0, 16, 16}, //8
    {0, 16, 16, 16},  //9  b
    {16, 16, 16, 16}, //10 bx
    {32, 16, 16, 16}, //11 ba
    {48, 16, 16, 16}, //12 ?
    {64, 16, 16, 16}, //13 flag
    {80, 16, 16, 16}, //14 filled
    {0, 32, 13, 23},  //15 numbers
    {13, 32, 13, 23}, //1
    {26, 32, 13, 23}, //2
    {39, 32, 13, 23}, //3
    {52, 32, 13, 23}, //4
    {65, 32, 13, 23}, //5
    {78, 32, 13, 23}, //6
    {91, 32, 13, 23}, //7
    {104, 32, 13, 23}, //8
    {117, 32, 13, 23}, //9
    {0, 55, 26, 26}, //smiley 25
    {26, 55, 26, 26}, //normal 26
    {52, 55, 26, 26}, //O.o 27
    {78, 55, 26, 26}, //x.x 28
    {104, 55, 26, 26} //cool 29
};

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

extern board b;
