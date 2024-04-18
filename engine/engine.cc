#include "engine.h"

Engine::Engine(bool isWhite, Game *g): Player(isWhite, g), board{Bitboards()}, e{Eval()}, engineIsWhite{isWhite} {}

void Engine::move(Move m) {
    int from = (7 - m.oldRow) * 8 + m.oldCol;
    int to = (7 - m.newRow) * 8 + m.newCol;
    int pieceType;
    vector<bitboard> curBoards = m.whiteMove ? board.whiteBoards : board.blackBoards;
    // determine piece type
    for(int i = 0; i < 6; i++) {
        if(curBoards[i] & (1ULL << from)) {
            pieceType = i;
            break;
        }
    }
    
    FastMove fm{from, to, m.whiteMove, pieceType};
    board.movePiece(fm);
}

char Engine::handlePawnPromotion() {
    // temp
    return 'Q';
}

Move Engine::getNextMove() {
    vector<FastMove> validMoves = board.getValidMoves(engineIsWhite);
    float maxEval = -1000000;
    FastMove bestMove;
    for(FastMove m : validMoves) {
        Bitboards newPosn{board};
        newPosn.movePiece(m);
        float eval = search(newPosn, 3, -1000000, 1000000, false);
        if(eval > maxEval) {
            maxEval = eval;
            bestMove = m;
        }
    }
    // convert to Move
    int oldCol = bestMove.from % 8;
    int oldRow = 7 - (bestMove.from / 8);
    int newCol = bestMove.to % 8;
    int newRow = 7 - (bestMove.to / 8);

    return {oldRow, oldCol, newRow, newCol, engineIsWhite};
}



float Engine::search(Bitboards posn, int depth, float alpha, float beta, bool maxPlayer) {
    if(depth == 0 || posn.getValidMoves(maxPlayer).size() == 0) {
        Eval e;
        return e.eval(posn);
    }

    if(maxPlayer) {
        float maxEval = -1000000;
        // for each move, calculate the eval
        for(FastMove m : posn.getValidMoves(engineIsWhite)) {
            Bitboards newPosn{posn};
            newPosn.movePiece(m);
            float eval = search(newPosn, depth - 1, alpha, beta, false);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        float minEval = 1000000;
        for(FastMove m : posn.getValidMoves(!engineIsWhite)) {
            Bitboards newPosn{posn};
            newPosn.movePiece(m);
            float eval = search(newPosn, depth - 1, alpha, beta, true);
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if(beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}
