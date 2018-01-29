#include "../include/main.h"

int main(int argc, char *argv[]){
    if(argc == 5){
        b.rown = atoi(argv[1]); b.coln = atoi(argv[2]); b.minen = atoi(argv[3]); b.limit = atoi(argv[4]);
        //SDL_Init
        SDL_Init(SDL_INIT_EVERYTHING);
        #ifdef __WIN32__
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
        #endif
        init();
        main_graphics();
        start();
    }
    char c;
    do{
        switch(c = choice()){
            //beginner
            case 1:
                b.hard = false;
                b.rown = 9; b.coln =  9; b.minen = 10; b.counter = 0; b.limit = 0;
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
