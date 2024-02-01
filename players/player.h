#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "../pieces/move.h"
#include "../game.h"

class Player {
    protected:
        bool isWhite;
        Game *g;


    public:
        Player(bool isWhite, Game* g); 
        virtual ~Player();
        virtual char handlePawnPromotion() = 0;
        virtual Move getNextMove() = 0;
        bool getIsWhite() const;
};

#endif
