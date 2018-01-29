#include "../include/graphics.h"

void draw(){
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
            if(b.arr[row][col] >= 10)
                SDL_BlitSurface( tileset, &clip[14], screen, &offset);

            //flagged
            if(b.arr[row][col] >= 20 && b.arr[row][col] < 30)
                SDL_BlitSurface( tileset, &clip[13], screen, &offset);

            //revealed
            if(b.arr[row][col] < 10)
                SDL_BlitSurface( tileset, &clip[b.arr[row][col]], screen, &offset);

            //bombred
            if(b.arr[row][col] == 40)
                SDL_BlitSurface( tileset, &clip[11], screen, &offset);

            //bomb
            if(b.arr[row][col] == 9)
                SDL_BlitSurface( tileset, &clip[9], screen, &offset);

            //bombx
            if(b.lose && b.arr[row][col] == 29)
                SDL_BlitSurface( tileset, &clip[10], screen, &offset);

            //?
            if(b.arr[row][col] >= 30 && b.arr[row][col] < 40 )
                SDL_BlitSurface( tileset, &clip[12], screen, &offset);
        }
    }
    SDL_Flip(screen);
}

int choice(){
    SDL_Event ev;
    char c = 0;

    //SDL_Init
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
    boxColor(screen, 10, 10, 9*FSIZ-10, 40, 0x00ff00ff);
    //hard
    boxColor(screen, 10, 50, 9*FSIZ-10, 80, 0xff5100ff);
    //custom
    boxColor(screen, 10, 90, 9*FSIZ-10, 120, 0x0055ffff);
    //load
    boxColor(screen, 10, 130, 9*FSIZ-10, 160, 0xffffffff);

    stringColor(screen, (9*FSIZ / 2) - 8*4, 20, "Beginner", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 4*4, 60, "Hard", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 6*4, 100, "Custom", 0x000000ff);
    stringColor(screen, (9*FSIZ / 2) - 4*4, 140, "Load", 0x000000ff);
    SDL_Flip(screen);

    while(!c){
        SDL_WaitEvent(&ev);
        switch (ev.type){
            case SDL_MOUSEBUTTONUP:
                if(ev.button.button == SDL_BUTTON_LEFT){
                    if(ev.button.x > 10 && ev.button.x < b.coln*FSIZ-10){
                        //beginner
                        if(ev.button.y > 10 && ev.button.y < 40)
                            c = 1;
                        //hard
                        if(ev.button.y > 50 && ev.button.y < 80)
                            c = 2;
                        //custom
                        if(ev.button.y > 90 && ev.button.y < 120)
                            c = 3;
                        //load
                        if(ev.button.y > 130 && ev.button.y < 160)
                            c = 4;
                    }
                }
                break;
            case SDL_QUIT:
                return -1;
        }
    }
    return c;
}

int custom(){
    SDL_Event ev;
    char c = 0;
    unsigned short rown = b.rown, coln = b.coln, minen = b.minen, limit = b.limit;
    char tmp[4];

    screen = SDL_SetVideoMode(9 * FSIZ, 11 * FSIZ + 20, 0, SDL_ANYFORMAT);
    SDL_WM_SetCaption("Minesweeper", "Minesweeper");

    //bg
    boxColor(screen, 0, 0, 9*FSIZ, 11*FSIZ + 40, 0xc0c0c0ff);
    //row
    boxColor(screen, 10, 10, 9*FSIZ-10, 40, 0xffffffff);
    //col
    boxColor(screen, 10, 50, 9*FSIZ-10, 80, 0xffffffff);
    //bomb
    boxColor(screen, 10, 90, 9*FSIZ-10, 120, 0xffffffff);
    //limit
    boxColor(screen, 10, 130, 9*FSIZ-10, 160, 0xffffffff);
    //ok
    boxColor(screen, ((9 * FSIZ) / 2) - 20, 170, ((9 * FSIZ) / 2) + 20 , 190, 0xffffffff);

    while(true){
        boxColor(screen, 10, 10, 9*FSIZ-10, 40, (c == 0) ? 0xff0000ff : 0xffffffff);
        boxColor(screen, 10, 50, 9*FSIZ-10, 80, (c == 1) ? 0xff0000ff : 0xffffffff);
        boxColor(screen, 10, 90, 9*FSIZ-10, 120, (c == 2) ? 0xff0000ff : 0xffffffff);
        boxColor(screen, 10, 130, 9*FSIZ-10, 160, (c == 3) ? 0xff0000ff : 0xffffffff);

        sprintf(tmp, "%d", rown);
        stringColor(screen, 10, 20, tmp, 0x000000ff);
        sprintf(tmp, "%d", coln);
        stringColor(screen, 10, 60, tmp, 0x000000ff);
        sprintf(tmp, "%d", minen);
        stringColor(screen, 10, 100, tmp, 0x000000ff);
        sprintf(tmp, "%d", limit);
        stringColor(screen, 10, 140, tmp, 0x000000ff);

        stringColor(screen, 9*FSIZ-10-23, 20, "row", 0x000000ff);
        stringColor(screen, 9*FSIZ-10-23, 60, "col", 0x000000ff);
        stringColor(screen, 9*FSIZ-10-40, 100, "minen", 0x000000ff);
        stringColor(screen, 9*FSIZ-10-40, 140, "limit", 0x000000ff);
        stringColor(screen, ((9 * FSIZ) / 2)-6, 175, "ok", 0x000000ff);
        SDL_Flip(screen);

        SDL_WaitEvent(&ev);
        switch (ev.type){
            case SDL_MOUSEBUTTONDOWN:
                if(ev.button.button == SDL_BUTTON_LEFT){
                    if(ev.button.x > 10 && ev.button.x < b.coln*FSIZ-10){
                        //row
                        if(ev.button.y > 10 && ev.button.y < 40)
                            c = 0;
                        //col
                        if(ev.button.y > 50 && ev.button.y < 80)
                            c = 1;
                        //bomb
                        if(ev.button.y > 90 && ev.button.y < 120)
                            c = 2;
                        //limit
                        if(ev.button.y > 130 && ev.button.y < 160)
                            c = 3;
                    //ok
                    if(ev.button.x > ((9 * FSIZ) / 2) - 20 && ev.button.x < ((9 * FSIZ) / 2) + 20){
                        if(ev.button.y > 170 && ev.button.y < 190){
                            if(rown < 9 || coln < 9 || minen < 5 || (minen >= b.rown*b.coln) || limit < 0 || limit > 999)
                                printf("Error!\n");
                            else{
                                b.rown = rown; b.coln = coln; b.minen = minen; b.limit = limit;
                                return 0;
                            }
                        }
                    }
                    }
                }
                break;
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym >= SDLK_0 && ev.key.keysym.sym <= SDLK_9){
                    switch(c){
                        case 0:
                            if((float)rown / 100 < 1.0) rown = rown*10 + ev.key.keysym.sym - SDLK_0;
                            break;
                        case 1:
                            if((float)coln / 100 < 1.0) coln = coln*10 + ev.key.keysym.sym - SDLK_0;
                            break;
                        case 2:
                            if((float)minen / 100 < 1.0) minen = minen*10 + ev.key.keysym.sym - SDLK_0;
                            break;
                        case 3:
                            if((float)limit / 100 < 1.0) limit = limit*10 + ev.key.keysym.sym - SDLK_0;
                            break;
                    }
                }
                if(ev.key.keysym.sym >= SDLK_KP0 && ev.key.keysym.sym <= SDLK_KP9){
                    switch(c){
                        case 0:
                            if((float)rown / 100 < 1.0) rown = rown*10 + ev.key.keysym.sym - SDLK_KP0;
                            break;
                        case 1:
                            if((float)coln / 100 < 1.0) coln = coln*10 + ev.key.keysym.sym - SDLK_KP0;
                            break;
                        case 2:
                            if((float)minen / 100 < 1.0) minen = minen*10 + ev.key.keysym.sym - SDLK_KP0;
                            break;
                        case 3:
                            if((float)limit / 100 < 1.0) limit = limit*10 + ev.key.keysym.sym - SDLK_KP0;
                            break;
                    }
                }
                if(ev.key.keysym.sym == SDLK_TAB){
                    if(c != 3)c++;
                    else c = 0;
                }
                if(ev.key.keysym.sym == SDLK_BACKSPACE){
                    switch(c){
                        case 0:
                            if(rown > 0) rown /= 10;
                            break;
                        case 1:
                            if(coln > 0) coln /= 10;
                            break;
                        case 2:
                            if(minen > 0) minen /= 10;
                            break;
                        case 3:
                            if(limit > 0) limit /= 10;
                            break;
                    }
                }
                if(ev.key.keysym.sym == SDLK_RETURN || ev.key.keysym.sym == SDLK_KP_ENTER){
                    if(rown < 9 || coln < 9 || minen < 5 || (minen >= b.rown*b.coln) || limit < 0 || limit > 999)
                        printf("Error!\n");
                    else{
                        b.rown = rown; b.coln = coln; b.minen = minen; b.limit = limit;
                    return 0;
                    }
                }
                break;
            case SDL_QUIT:
                return 1;
                break;
        }
    }
}

Uint32 draw_time(Uint32 ms, void *param){
    SDL_Event ev;
    SDL_Rect offset = {0, 0, 0, 0};
    //time numbers
    offset.y = 17;
    offset.x = (b.coln*FSIZ)-BSIZ;
    if(b.limit && b.counter >= 0)
        b.counter -= ms / 1000;
    else if(!b.limit)
        b.counter += ms / 1000;

    //lose condition
    if((b.limit && !b.counter) || b.counter == 999){
        ev.type = SDL_USEREVENT;
        SDL_PushEvent(&ev);
        b.lose = true;
        b.smiley = 2;
        draw();
    }

    int num = b.counter;

    //1
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    num /= 10;

    //10
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+(num % 10)], screen, &offset);
    num /= 10;

    //100
    offset.x -= 13;
    SDL_BlitSurface( tileset, &clip[15+num], screen, &offset);
    SDL_Flip(screen);
    return ms;
}

void main_graphics(){
    printf("Init main graphics!\n");
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
    draw();
}
