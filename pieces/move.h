#ifndef __MOVE_H__
#define __MOVE_H__

using namespace std;

enum class PieceType {
    Pawn, Bishop, Knight, Rook, Queen, King
};

class Move {
    public:
        int oldCol;
        int oldRow;
        int newCol;
        int newRow;
        bool whiteMove;
        
        Move(): oldCol {-1}, oldRow {-1}, newCol {-1}, newRow {-1}, whiteMove {false} {}

        Move(int oldCol, int oldRow, int newCol, int newRow, bool whiteMove):
            oldCol {oldCol}, oldRow {oldRow}, newCol {newCol}, newRow {newRow}, whiteMove {whiteMove} {}
        bool operator==(Move &other) {
            return (oldCol == other.oldCol) && (oldRow == other.oldRow)
                && (newCol == other.newCol) && (newRow == other.newRow)
                && (whiteMove == other.whiteMove);
        }
};

#endif
