#include "logic.h"

//init the board
void init(){
    printf("Init!\n");
    b.mine = b.minen;
    b.counter = (b.counter) ? b.counter : b.limit;
    b.arr = (int **)malloc(b.rown * sizeof(int*));
    if(b.arr != NULL){
        b.arr[0] = (int *)malloc(b.rown*b.coln * sizeof(int));
        if(b.arr[0] != NULL){
            int row, col;
            for(row = 1; row < b.rown; row++){
               b.arr[row] = b.arr[0] + row*b.coln;
            }
            for(row = 0; row < b.rown; row++){
                for(col = 0; col < b.coln; col++){
                    b.arr[row][col] = 10;
                }
            }
        }
        else{
            printf("Failed to create fields!\n");
            perror("Cannot create fields!\n");
            exit(1);
        }
    }
    else{
        printf("Failed to init the board!\n");
        perror("Cannot create board!\n");
        exit(1);
    }
}

//save game
void save(){
    FILE* fp;
    fp = fopen("save", "wb");
    printf("Saving!\n");
    if(fp != NULL) {
        fwrite(&b, sizeof(board), 1, fp);
        int row, col;
        for(row = 0; row < b.rown; row++){
            for(col = 0; col < b.coln; col++){
                fwrite(&b.arr[row][col], sizeof(int), 1, fp);
            }
        }
        printf("Saved!\n");
        fflush(fp);
    }
    else{
        printf("Failed to save!\n");
        perror("Cannot open file\n");
        exit(1);
    }
    fclose(fp);
}

//load game
void load(){
    FILE* fp;
    fp = fopen("save", "rb");
    printf("Loading!\n");
    if(fp != NULL){
        if(!b.generated){
            fread(&b, sizeof(board), 1, fp);
            init();
        }
        else{
            board tmp;
            fread(&tmp, sizeof(board), 1, fp);

            b.mine = tmp.mine;
            b.mflagged = tmp.mflagged;
            b.counter = tmp.counter;
            b.revealed = tmp.revealed;
        }
        printf("Reading File!\n");
        int row, col;
        for(row = 0; row < b.rown; row++){
            for(col = 0; col < b.coln; col++){
                fread(&b.arr[row][col], sizeof(int), 1, fp);
            }
        }
    }
    else {
        printf("Failed to load!\n");
        perror("Cannot open file\n");
        exit(1);
    }
    b.generated = true;
    b.loaded = true;
    fclose(fp);
    draw();
}

//add 1 to field
void add(int row, int col){
    if(row < 0 || row >= b.rown) return;
    if(col < 0 || col >= b.coln) return;
    if(b.arr[row][col] % 10 == 9) return;
    b.arr[row][col]++;
}

//board generation
void gen_board(int r, int c){
    printf("Generated!\n");
    srand((unsigned)time(NULL));
    int n = 0, row, col;
    while(n < b.minen){
        row = rand() % b.rown;
        col = rand() % b.coln;
        if(!b.hard && (row == r && col == c)) continue;
        if(b.arr[row][col] % 10 != 9){
            b.arr[row][col] = 19;
            add(row + 1, col);
            add(row - 1, col);
            add(row, col + 1);
            add(row, col - 1);
            add(row - 1, col + 1);
            add(row + 1, col + 1);
            add(row - 1, col - 1);
            add(row + 1, col - 1);
            n++;
        }
    }
    b.generated = true;
    save();
}

//reset game
void reset(){
    printf("Resetting!\n");
    b.mine = b.minen;
    b.lose = false;
    b.win = false;
    b.generated = false;
    b.loaded = false;
    b.mflagged = 0;
    b.smiley = 0;
    b.counter = (b.limit) ? b.limit : 0;
    b.revealed = 0;
    int row, col;
    for(row = 0; row < b.rown; row++){
        for(col = 0; col < b.coln; col++){
            b.arr[row][col] = 10;
        }
    }
}

//field flag
void flag(int row, int col, bool q){
    //set field from question mark to normal
    if(q && b.arr[row][col] >= 30){
        b.arr[row][col] -= 20;
        return;
    }
    //set field to flagged or question mark
    if(b.arr[row][col] >= 10 && b.arr[row][col] < 20){
        //question mark
        if(q){
            b.arr[row][col] += 20;
            return;
        }
        //flagged
        b.arr[row][col] += 10;
        if(b.mine > 0)
            b.mine--;
        if(b.arr[row][col] % 10 == 9)
            b.mflagged++;
    }
    //set field to normal
    else if(b.arr[row][col] >= 20 && b.arr[row][col] < 30){
        b.arr[row][col] -= 10;
        if(b.mine < b.minen)
            b.mine++;
        if((b.arr[row][col] % 10) == 9)
            b.mflagged--;
    }
}

//reveal one field
void reveal(int row, int col){
    if(row < 0 || row >= b.rown) return;
    if(col < 0 || col >= b.coln) return;
    if(b.arr[row][col] < 10)  return;
    if(b.arr[row][col] >= 20) return;
    //lose condition
    if(b.arr[row][col] % 10 == 9){
        b.arr[row][col] = 40;
        printf("Boom!\n");
        int r, c;
        for(r = 0; r < b.rown; r++){
            for(c = 0; c < b.coln; c++){
                if(b.arr[r][c] != 40 && b.arr[r][c] != 29){
                    b.arr[r][c] %= 10;
                }
            }
        }
        b.smiley = 2;
        b.lose = true;
    }
    else{
        b.revealed++;
        b.arr[row][col] %= 10;
        //win condition
        if(b.revealed == (b.rown * b.coln) - b.minen){
            b.win = true;
            return;
        }
        if(b.arr[row][col] == 0){
            reveal(row + 1, col);
            reveal(row - 1, col);
            reveal(row, col + 1);
            reveal(row, col - 1);
        }
    }
}

//game loop
void start(){
    printf("Started!\n");

    //init vars
    SDL_TimerID id;
    SDL_Event ev;
    bool quit = false;
    unsigned int col, row;

    while(!quit){
        SDL_WaitEvent(&ev);
        switch (ev.type){
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym == SDLK_F1)
                    save();
                if(ev.key.keysym.sym == SDLK_F2){
                    if(id) SDL_RemoveTimer(id);
                    load();
                }
                if(ev.key.keysym.sym == SDLK_F3){
                    printf("\nmine:%d\nmflagged:%d\ncounter:%d\nlose:%d\nwin:%d\ngenerated:%d\nsmiley:%d\nloaded:%d\nrevealed:%d\nlimit%d\n",b.mine,b.mflagged,b.counter,b.lose,b.win,b.generated,b.smiley, b.loaded, b.revealed, b.limit);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                row = floor((double)(ev.button.y-56) / FSIZ);
                col = floor((double)(ev.button.x-BSIZ) / FSIZ);
                if(ev.button.y > 56 && ev.button.y < 56 + b.rown * FSIZ && (!b.lose && !b.win) && ev.button.x > BSIZ && ev.button.x < b.coln*FSIZ+BSIZ){
                    if(ev.button.button == SDL_BUTTON_MIDDLE)
                        flag(row, col, 1);
                    if(ev.button.button == SDL_BUTTON_LEFT){
                        if (!b.generated){
                            gen_board(row, col);
                            id = SDL_AddTimer(1000, draw_time, NULL);
                        }
                        if (b.loaded){
                            if(id) SDL_RemoveTimer(id);
                            b.loaded = false;
                            id = SDL_AddTimer(1000, draw_time, NULL);
                        }
                        if(b.arr[row][col] >= 10 && b.arr[row][col] < 20)
                            b.smiley = 1;
                    }
                }
                if(ev.button.y >= BSIZ+5 && ev.button.y <= 41 && ev.button.x >= ((b.coln*FSIZ+2*BSIZ) / 2)-13 && ev.button.x <= ((b.coln*FSIZ+2*BSIZ) / 2)+13){
                    b.smiley = -1;
                }
                draw();
                break;
            case SDL_MOUSEBUTTONUP:
                row = floor((double)(ev.button.y-56) / FSIZ);
                col = floor((double)(ev.button.x-10) / FSIZ);
                if(ev.button.button == SDL_BUTTON_LEFT){
                    if(ev.button.y >= BSIZ+5 && ev.button.y <= 41 && ev.button.x >= ((b.coln*FSIZ+2*BSIZ) / 2)-13 && ev.button.x <= ((b.coln*FSIZ+2*BSIZ) / 2)+13){
                        if(id) SDL_RemoveTimer(id);
                        reset();
                    }
                }
                if(ev.button.y > 56 && ev.button.y < 56 + b.rown * FSIZ && (!b.lose && !b.win) && b.generated && ev.button.x > BSIZ && ev.button.x < b.coln*FSIZ+BSIZ){
                    b.smiley = 0;
                    if(ev.button.button == SDL_BUTTON_LEFT){
                        reveal(row, col);
                    }

                    if(ev.button.button == SDL_BUTTON_RIGHT && b.arr[row][col] >= 10){
                        flag(row, col, 0);
                    }
                }
                draw();
                break;
            case SDL_QUIT:
                quit = true;
                break;
        }
        if(b.lose || b.win)
            SDL_RemoveTimer(id);
        //win condition
        if(b.mflagged == b.minen || b.win){
            b.smiley = 3;
            b.win = true;
            draw();
        }
    }

    //outside the game loop
    reset();
    if(id) SDL_RemoveTimer(id);
    free(b.arr);
    free(b.arr[0]);
}
