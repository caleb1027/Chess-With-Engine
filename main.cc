#include "game.h"
#include "board/board.h"
#include "views/textview.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc > 1) {
        Game g{false};
        g.play();
    } else {
        Game g{true};
        g.play();
    }
}
