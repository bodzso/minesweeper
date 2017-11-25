#include "../include/main.h"

int main(int argc, char *argv[]){
    if(argc > 1){
        b.rown = atoi(argv[1]); b.coln = atoi(argv[2]); b.minen = atoi(argv[3]); b.limit = atoi(argv[4]);
        init();
        init_graphics();
        start();
    }
    short c;
    do{
        switch(c = choice()){
            //custom
            case 1:
                printf("Enter row(min 9) col(min 9) mine(min 5) limit(>=0 & <999): ");
                scanf("%d %d %d %d", &b.rown, &b.coln, &b.minen, &b.limit);
                if(b.rown < 9 || b.coln < 9 || b.minen < 5 || (b.minen >= b.rown*b.coln) || b.limit < 0 || b.limit > 999){
                    printf("Usage: row(min 9) col(min 9) mine(min 5) limit(>=0 & <999)\n");
                    continue;
                }
                init();
                init_graphics();
                start();
                break;
            //beginner
            case 2:
                b.rown = 9; b.coln =  9; b.minen = 10; b.limit = 0;
                init();
                init_graphics();
                start();
                break;
            //hard
            case 3:
                b.rown = 15; b.coln =  15; b.minen = 20; b.limit = 0;
                b.hard = true;
                init();
                init_graphics();
                start();
                break;
            //load
            case 4:
                b.rown = 9; b.coln =  9; b.minen = 10; b.limit = 0;
                load();
                init_graphics();
                start();
                break;
        }
    }while(c != -1);

    return 0;
}
