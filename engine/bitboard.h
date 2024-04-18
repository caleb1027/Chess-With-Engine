#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../board/board.h"


using namespace std;

// board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};



/* Board Layout
"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
*/

using bitboard = unsigned long long;

// Macros, faster than functions
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

// only stores from and to squares, as well as color of piece
// we need nothing else if we only use bitmaps to generate moves
struct FastMove {
    int from;
    int to;
    bool isWhite;
    int pieceType;
};

enum {
    Pawn, Knight, Bishop, Rook, Queen, King
};

class Bitboards {
    private:
        // precalculated move tables for pawns (index 0/false: black, index 1/true: white)
        static bitboard pawnMoveMasks[2][64];
        // precalculated move table for knights
        static bitboard knightMoveMasks[64];
        // precalculated move table for king
        static bitboard kingMoveMasks[64];
        static bitboard horizontalBoundary;
        static bitboard verticalBoundary;
        // bitboard of all occupied squares
        bitboard occupiedBlack;
        bitboard occupiedWhite;
        bitboard firstMove; // for castling, en passent, etc.

        // checks if square is occupied by own piece
        bool isOccupied(bool isWhite, int to);
        bool isCapture(FastMove m);
        // if checkWhite = true, checks if white is in check
        bool inCheck(bool checkWhite);
        bool inCheckmate(bool checkWhite);
        vector<FastMove> getMovesDirection(bitboard positions, const bitboard &occupiedByOther, bool getWhiteMoves, int direction, int pieceType);
        vector<FastMove> getValidMovesPawn(bool getWhiteMoves);
        vector<FastMove> getValidMovesKnight(bool getWhiteMoves);
        vector<FastMove> getValidMovesBishop(bool getWhiteMoves);
        vector<FastMove> getValidMovesRook(bool getWhiteMoves);
        vector<FastMove> getValidMovesQueen(bool getWhiteMoves); 
        vector<FastMove> getValidMovesKing(bool getWhiteMoves);
        void updateNonPieceBoards();
    public:
        // bitboard construct w/ default board (game start)
        Bitboards();
        // bitboard constructor w/ FEN string
        Bitboards(string fen);
        static void printBitboard(bitboard b);
        void printAsBoard();
        void movePiece(FastMove m);
        bool validateMove(FastMove m, bool whiteToMove);
        vector<FastMove> getValidMoves(bool getWhiteMoves);
        static void generateMoveTables();
        void initFENBoard(string fen);
        // order of bitboard vector: pawns, knights, bishops, rooks, queens, kings
        vector<bitboard> whiteBoards;
        vector<bitboard> blackBoards;
        bool whiteToMove;
        bool whiteCastleK;
        bool whiteCastleQ;
        bool blackCastleK;
        bool blackCastleQ;
        // stores all valid en passent destinations
        bitboard validEnPassentDestinations;
        int halfMovesSinceCapture;
        int halfMovesTotal;
};

bitboard Bitboards::pawnMoveMasks[2][64];
// precalculated move table for knights
bitboard Bitboards::knightMoveMasks[64];
// precalculated move table for king
bitboard Bitboards::kingMoveMasks[64];
bitboard Bitboards::horizontalBoundary;
bitboard Bitboards::verticalBoundary;

#endif
