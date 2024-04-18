#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "bitboard.h"
#include "eval.h"
#include "../players/player.h"

class Engine: public Player {
    private:
        Bitboards board;
        Eval e;
        bool engineIsWhite;
        float search(Bitboards posn, int depth, float alpha, float beta, bool maxPlayer);
    public:
        Engine(bool isWhite, Game *g);
        void move(Move m);
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

#endif