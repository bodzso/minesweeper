#include "../include/graphics.h"

void paint(){
    int row, col;
    SDL_Rect offset = {0, 0, 0, 0};

    //mine numbers
    offset.x = 45;
    offset.y = 17;

    int num = b.mine;

    //1
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    num /= 10;

    //10
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    if(num > 0)
        num /= 10;

    //100
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+num], screen, &offset);

    //smiley
    offset.x = ((b.coln * FSIZ + 20) / 2)-13;
    SDL_BlitSurface( tileset, &clip[26+b.smiley], screen, &offset);

    //time
    if(!b.generated || b.loaded){
        offset.x = (b.coln * FSIZ) - BSIZ;
        num = b.counter;

        //1
        SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
        num /= 10;

        //10
        offset.x -= 13;
        SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
        if(num > 0)
            num /= 10;

        //100
        offset.x -= 13;
        SDL_BlitSurface( tileset, &clip[15+num], screen, &offset);
    }

    //fields
    for(row = 0; row < b.rown; row++){
        for(col = 0; col < b.coln; col++){
            offset.x = col * FSIZ + BSIZ;
            offset.y = row * FSIZ + 56;

            //hidden
            if(b.arr[row][col] >= 10){
                SDL_BlitSurface( tileset, &clip[14], screen, &offset);
            }

            //flagged
            if(b.arr[row][col] >= 20 && b.arr[row][col] < 30){
                SDL_BlitSurface( tileset, &clip[13], screen, &offset);
            }

            //revealed
            if(b.arr[row][col] < 10){
                SDL_BlitSurface( tileset, &clip[b.arr[row][col]], screen, &offset);
            }

            //bombred
            if(b.arr[row][col] == 40){
                SDL_BlitSurface( tileset, &clip[11], screen, &offset);
            }

            //bomb
            if(b.arr[row][col] == 9){
                SDL_BlitSurface( tileset, &clip[9], screen, &offset);
            }

            //bombx
            if(b.loose && b.arr[row][col] > 20 && (b.arr[row][col] % 10 == 9)){
                SDL_BlitSurface( tileset, &clip[10], screen, &offset);
            }

            //?
            if(b.arr[row][col] >= 30 && b.arr[row][col] < 40 ){
                SDL_BlitSurface( tileset, &clip[12], screen, &offset);
            }
        }
    }
    SDL_Flip(screen);
}

int choice(){
    SDL_Event ev;
    bool quit = false;
    int c = 0;

    SDL_Init(SDL_INIT_EVERYTHING);
    #ifdef __WIN32__
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    #endif
    screen = SDL_SetVideoMode(9 * FSIZ, 11 * FSIZ, 0, SDL_ANYFORMAT);
    SDL_WM_SetCaption("Minesweeper", "Minesweeper");

    //bg
    boxColor(screen, 0, 0, 9*FSIZ, 11*FSIZ, 0xc0c0c0ff);

    //beginner
    boxColor(screen, 10, 10, 9*FSIZ-10, 40, 0xffffffff);

    //hard
    boxColor(screen, 10, 50, 9*FSIZ-10, 80, 0xffffffff);

    //custom
    boxColor(screen, 10, 90, 9*FSIZ-10, 120, 0xffffffff);

    //load
    boxColor(screen, 10, 130, 9*FSIZ-10, 160, 0xffffffff);

    stringColor(screen, (9*FSIZ / 2) - 8*4, 20, "Beginner", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 4*4, 60, "Hard", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 6*4, 100, "Custom", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 4*4, 140, "Load", 0x000000ff);
    SDL_Flip(screen);

    while(!quit && !c){
        SDL_WaitEvent(&ev);
        switch (ev.type){
            case SDL_MOUSEBUTTONUP:
                if(ev.button.button == SDL_BUTTON_LEFT){
                    if(ev.button.x > 10 && ev.button.x < b.coln*FSIZ-10){
                        //beginner
                        if(ev.button.y > 10 && ev.button.y < 40)
                            c = 2;
                        //hard
                        if(ev.button.y > 50 && ev.button.y < 80)
                            c = 3;
                        //custom
                        if(ev.button.y > 90 && ev.button.y < 120)
                            c = 1;
                        //load
                        if(ev.button.y > 130 && ev.button.y < 160)
                            c = 4;
                    }
                }
                break;
            case SDL_QUIT:
                quit = true;
                c = -1;
                break;
        }
    }
    SDL_FreeSurface(screen);
    return c;
}

Uint32 draw_time(Uint32 ms, void *param){
    SDL_Rect offset = {0, 0, 0, 0};
    offset.y = 17;
    offset.x = (b.coln*FSIZ)-BSIZ;
    if(b.limit && b.counter > 0){
        b.counter -= ms / 1000;
    }
    else if(!b.limit)
        b.counter += ms / 1000;

    if(b.limit && !b.counter){
        b.loose = true;
        b.smiley = 2;
        paint();
    }

    int num = b.counter;

    //1
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    num /= 10;

    //10
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    if(num > 0){
        num /= 10;
    }

    //100
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+num], screen, &offset);
    SDL_Flip(screen);
    return ms;
}

void init_graphics(){
    printf("Init graphics!\n");
    tileset = IMG_Load("tileset.png");
    if(!tileset){
        perror("Cannot open tileset.png!\n");
        printf("%s\n", IMG_GetError());
        exit(1);
    }

    screen = SDL_SetVideoMode(b.coln * FSIZ + 20, b.rown * FSIZ + 66, 0, SDL_ANYFORMAT);
    if(!screen) {
        perror("Cannot open window!\n");
        exit(1);
    }

    //backgrounds
    //main
    boxColor(screen, 0, 0, b.coln*FSIZ + 20, b.rown*FSIZ + 66, 0xc0c0c0ff);
    //state
    boxColor(screen, BSIZ, BSIZ, b.coln*FSIZ+BSIZ, 46, 0xc9c9c9ff);

    //horizontal grey
    thickLineColor(screen, BSIZ, BSIZ, b.coln * FSIZ + BSIZ-2, BSIZ, 2, 0x808080ff);
    thickLineColor(screen, BSIZ, 55, b.coln * FSIZ + BSIZ, 55, 2, 0x808080ff);

    //vertical grey
    thickLineColor(screen, BSIZ, BSIZ, BSIZ, 44, 2, 0x808080ff);
    thickLineColor(screen, 9, 54, 9, b.rown * FSIZ + 54, 2, 0x808080ff);

    //horizontal wh
    thickLineColor(screen, BSIZ, 46, b.coln * FSIZ + BSIZ, 46, 2, 0xffffffff);
    thickLineColor(screen, BSIZ, b.rown * FSIZ + 56, b.coln * FSIZ + BSIZ, b.rown * FSIZ + 56, 2, 0xffffffff);

    //vertical wh
    thickLineColor(screen, b.coln * FSIZ + BSIZ, 46, b.coln * FSIZ + BSIZ, BSIZ, 2, 0xffffffff);
    thickLineColor(screen, b.coln * FSIZ + 11, b.rown * FSIZ + 56, b.coln * FSIZ + 11, 54, 2, 0xffffffff);
    paint();
}
