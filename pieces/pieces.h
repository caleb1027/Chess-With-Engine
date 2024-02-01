#ifndef __PIECES_H__
#define __PIECES_H__
#include "piece.h"

class Pawn: public Piece {
    bool moveDown;
    // need code to handle en passent, probably going to be in board
    public:
        Pawn(int col, int row, bool isWhite);
        Pawn(bool isWhite);
        bool validateMove(Move m) override;
};

class Rook: public Piece {
    public:
        Rook(int col, int row, bool isWhite);
        Rook(bool isWhite);
        bool validateMove(Move m) override;
};

class Bishop: public Piece {
    public:
        Bishop(int col, int row, bool isWhite);
        Bishop(bool isWhite);
        bool validateMove(Move m) override;
};

class Knight: public Piece {
    public:
        Knight(int col, int row, bool isWhite);
        Knight(bool isWhite);
        bool validateMove(Move m) override;
};

class Queen: public Piece {
    public:
        Queen(int col, int row, bool isWhite);
        Queen(bool isWhite);
        bool validateMove(Move m) override;
};

class King: public Piece {
    public:
        King(int col, int row, bool isWhite);
        King(bool isWhite);
        bool validateMove(Move m) override;
};

#endif
