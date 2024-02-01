#ifndef __HUMANPLAYER_H__
#define __HUMANPLAYER_H__

#include "player.h"

class HumanPlayer: public Player {
    public:
        HumanPlayer(bool isWhite, Game *g);
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

#endif
