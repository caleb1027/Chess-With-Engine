#ifndef __PIECE_H__
#define __PIECE_H__
#include "move.h"

class Piece {
    int col;
    int row;
    bool isWhite;
    bool captured;
    bool hasMoved;
    PieceType type;
   
    public:
        Piece(int col, int row, bool isWhite, PieceType type);
        Piece(bool isWhite, PieceType type);
        virtual ~Piece(){}
        int getCol();
        int getRow();
        PieceType getPieceType();
        bool getIsCaptured();
        void setIsCaptured(bool capture);
        bool getHasMoved();
        void setHasMoved(bool moved);
        bool getIsWhite();
        void move(Move m);
        Piece &operator=(Piece &other);
        virtual bool validateMove(Move m) = 0;
};

#endif
