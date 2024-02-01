#include "pieces.h"
#include <cstdlib>
#include <iostream>

using namespace std;
// CONSTRUCTORS:

Pawn::Pawn(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::Pawn}, moveDown{!isWhite} {} 

Pawn::Pawn(bool isWhite):
Piece{isWhite, PieceType::Pawn}, moveDown{!isWhite} {} 

Rook::Rook(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::Rook} {}

Rook::Rook(bool isWhite):
Piece{isWhite, PieceType::Rook} {} 

Bishop::Bishop(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::Bishop} {}

Bishop::Bishop(bool isWhite):
Piece{isWhite, PieceType::Bishop} {} 

Knight::Knight(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::Knight} {}

Knight::Knight(bool isWhite):
Piece{isWhite, PieceType::Knight} {} 

Queen::Queen(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::Queen} {}

Queen::Queen(bool isWhite):
Piece{isWhite, PieceType::Queen} {} 

King::King(int col, int row, bool isWhite):
Piece{col, row, isWhite, PieceType::King} {}

King::King(bool isWhite):
Piece{isWhite, PieceType::King} {} 

// MOVE VALIDATION:
// NOTE: GAME SHOULD HANDLE ENSURING THE MOVE ISN'T TO SAME SQUARE / OUT OF BOUNDS

bool Pawn::validateMove(Move m) {
    int direction = -1 * (moveDown ? 1 : -1); // will multiply by -1 if moving down

    if(abs(m.newCol - m.oldCol) > 1) {
        return false;
    }
    // moves in diagonal
    if(abs(m.newCol - m.oldCol) == 1 && m.newRow == (m.oldRow + direction)) {
        return true;
    }

    // account for ability to move 2 squares on first move
    if(!getHasMoved()) {
        if(moveDown) {
            if(m.oldRow > m.newRow && m.newRow >= m.oldRow + 2 * direction && m.oldCol == m.newCol) {
                return true;
            }
        } else {
            if(m.oldRow < m.newRow && m.newRow <= m.oldRow + 2 * direction && m.oldCol == m.newCol) {
                return true;
            }
        }
    } else {
        if(moveDown && m.oldRow > m.newRow && m.newRow >= m.oldRow + direction) {
                return true;
        } else if(m.oldRow < m.newRow && m.newRow <= m.oldRow + direction) {
            return true;
        }
    }
    return false; 
}

bool Rook::validateMove(Move m) {
    // check if col changes, y doesn't, vice versa
    // move checking done in controller ensures that a move isn't played to the same square
    if(m.oldCol == m.newCol || m.oldRow == m.newRow) {
        return true;
    }
    return false;
}

bool Bishop::validateMove(Move m) {
    // check if moving in a diagonal (same change in col and y)
    if(abs(m.newCol - m.oldCol) == abs(m.newRow - m.oldRow)) {
        return true;
    }
    return false;
}

bool Knight::validateMove(Move m) {
    // 2 in a direction, 1 in other
    if((abs(m.newCol - m.oldCol) == 2 && abs(m.newRow - m.oldRow) == 1)
    || (abs(m.newCol - m.oldCol) == 1 && abs(m.newRow - m.oldRow) == 2)) {
        return true;
    }
    return false;
}

bool King::validateMove(Move m) {
    // account for castling (can move 2 squares or click on rook)
    if(!getHasMoved() && m.oldRow == m.newRow && (abs(m.newCol - m.oldCol) == 2)) {
        return true;
    }

    // square around king
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            if(m.newCol == m.oldCol + i && m.newRow == m.oldRow + j) return true;
        }
    }
    return false;
}

bool Queen::validateMove(Move m) {
    // moves like a combination of rook and bishop
    if(m.oldCol == m.newCol || m.oldRow == m.newRow) {
        return true;
    }
    if(abs(m.newCol - m.oldCol) == abs(m.newRow - m.oldRow)) {
        return true;
    }
    return false;
}
