#include "humanplayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(bool isWhite, Game *g): Player{isWhite, g} {}
Move HumanPlayer::getNextMove() {
    char col;
    int oldCol;
    int oldRow;
    int newCol;
    int newRow;
    
    // NOTE: add error handling?
    // letter denotes col
    
    cin >> col >> oldRow;
    oldCol = col - 'a';
    oldRow--;

    cin >> col >> newRow;
    newCol = col - 'a';
    newRow--;
    return {oldCol, oldRow, newCol, newRow, isWhite};
}

char HumanPlayer::handlePawnPromotion() {
    cout << "Enter a character to promote pawn to: (K for knight, Q for queen, R for rook, B for bishop)" << endl;
    char type;
    cin >> type;
    return type;
}
