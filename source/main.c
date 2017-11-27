#include "../include/main.h"

int main(int argc, char *argv[]){
    char c;
    do{
        switch(c = choice()){
            //beginner
            case 1:
                b.rown = 9; b.coln =  9; b.minen = 10; b.limit = 0;
                init();
                main_graphics();
                start();
                break;
            //hard
            case 2:
                b.rown = 15; b.coln =  15; b.minen = 40; b.limit = 0;
                b.hard = true;
                init();
                main_graphics();
                start();
                break;
            //custom
            case 3:
                if(!custom()){
                    init();
                    main_graphics();
                    start();
                }
                break;
            //load
            case 4:
                load();
                main_graphics();
                start();
                break;
        }
    }while(c != -1);

    SDL_FreeSurface(tileset);
    SDL_FreeSurface(screen);
    SDL_Quit();
    return 0;
}
