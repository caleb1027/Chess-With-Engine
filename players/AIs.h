#ifndef __AIs_h__
#define __AIs_h__

#include "player.h"
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;


class AIPlayer: public Player {
    protected:
        // Generates a random integer from 0 to (n - 1) inclusive
        int generateRandomInt(size_t n);
        AIPlayer(bool isWhite, Game *g): Player(isWhite, g) {}
};

// Computer Level 1
class EasyAI: public AIPlayer {
    public:
        EasyAI(bool isWhite, Game *g): AIPlayer{isWhite, g} {}
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

// Computer Level 2
class MediumAI: public AIPlayer {
    public:
        MediumAI(bool isWhite, Game *g): AIPlayer{isWhite, g} {}  
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

// Computer Level 3
class HardAI: public AIPlayer {
    public:
        HardAI(bool isWhite, Game *g): AIPlayer{isWhite, g} {}  
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

// Computer Level 4
class BestAI: public AIPlayer {
    int determineMoveScore(Move m);
    public:
        BestAI(bool isWhite, Game *g): AIPlayer{isWhite, g} {}  
        Move getNextMove() override;
        char handlePawnPromotion() override;
};

#endif
