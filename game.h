#ifndef __GAME_H__
#define __GAME_H__

#include "board/board.h"
#include "pieces/move.h"
#include "views/textview.h"
#include "views/graphicsview.h"


using namespace std;

class Player;

class Game {
    Board board;
    bool isSetup;
    bool whiteToMove;
    float whiteScore;
    float blackScore;
    vector<unique_ptr<View>> views;
    vector<Move> moveList;

    void setupAdd();
    bool setupValid();
    // pseudovalid move validation for board b
    bool validateMove(Board b, Move m, bool checkColour);
    vector<int> findKing(Board b, bool white);
    void handleGameEnd(bool playing);

    public:
        unique_ptr<Player> p1;
        unique_ptr<Player> p2;

        Game(bool useGUI);
        ~Game();
        void updateView(); // updates view
        bool move(Move m); // if move is valid, makes move
        void play();
        vector<Move> generateValidMoves(bool whiteTurn);
        vector<Move> generateValidMoves(Board b, bool whiteTurn);
        void updateMoveList(Move m);
        Board getBoard();
        // checks board b for check of specified color (white = true checks if white is in check)
        bool inCheck(Board b, bool white);
        vector<vector<bool>> getHasMovedStates();
        void setHasMovedStates(vector<vector<bool>> states);
        vector<vector<bool>> getHasMovedStates(Board b);
        void setHasMovedStates(Board b, vector<vector<bool>> states);
        // checks board b for checkmate
        bool inCheckmate(Board b, bool white);
        bool inStalemate(bool white);
};

#endif
