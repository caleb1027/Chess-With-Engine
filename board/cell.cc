#include "cell.h"

using namespace std;

Cell::Cell(int row, int col, bool isWhite, bool isEmpty, shared_ptr<Piece> piece):
    row {row}, col {col}, isWhite {isWhite}, isEmpty {isEmpty}, piece {piece} {}

Cell& Cell::operator=(const Cell& other) {
    isEmpty = other.getIsEmpty();
    piece = other.getPiece();
    return *this;
}

int Cell::getRow() const {
    return row;
}

int Cell::getCol() const {
    return col;
}

bool Cell::getIsWhite() const {
    return isWhite;
}

bool Cell::getIsEmpty() const {
    return isEmpty;
}

void Cell::setIsEmpty(bool empty) {
    isEmpty = empty;
}

shared_ptr<Piece> Cell::getPiece() const{
    return piece;
}

void Cell::setPiece(shared_ptr<Piece> p) {
    piece = p;
}
