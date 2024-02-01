#include "piece.h"

Piece::Piece(int col, int row, bool isWhite, PieceType type):
col{col}, row{row}, isWhite{isWhite}, captured{false}, hasMoved{false}, type{type} {}

Piece::Piece(bool isWhite, PieceType type):
col{-1}, row{-1}, isWhite{isWhite}, captured{false}, hasMoved{false}, type{type} {}

Piece &Piece::operator=(Piece &other) {
    col = other.col;
    row = other.row;
    isWhite = other.isWhite;
    captured = other.captured;
    hasMoved = other.hasMoved;
    type = other.type;
    return *this;
}

void Piece::move(Move m) {
    col = m.newCol;
    row = m.newRow;
    setHasMoved(true);
}

int Piece::getCol() { return col; }
int Piece::getRow() { return row; }
PieceType Piece::getPieceType() { return type; }
bool Piece::getIsCaptured() { return captured; }
void Piece::setIsCaptured(bool capture) { captured = capture; }
bool Piece::getIsWhite() { return isWhite; }
bool Piece::getHasMoved() { return hasMoved; }
void Piece::setHasMoved(bool moved) { hasMoved = moved; }
