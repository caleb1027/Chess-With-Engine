#include "AIs.h"

int AIPlayer::generateRandomInt(size_t n) {
    if (n == 0) {
        throw logic_error {"generateRandomInt: n must be > 0"};
    }

    srand(time(nullptr));
    return rand() % n;
}

Move EasyAI::getNextMove() {
    vector<Move> possibleMoves = g->generateValidMoves(isWhite);
    size_t possibleMovesLength = possibleMoves.size();

    if (possibleMovesLength == 0) {
        throw logic_error {"No possible moves for Easy AI to choose from"};
    }

    return possibleMoves[generateRandomInt(possibleMovesLength)];
}

Move MediumAI::getNextMove() {
    vector<Move> possibleMoves = g->generateValidMoves(isWhite);
    size_t possibleMovesLength = possibleMoves.size();

    if (possibleMovesLength == 0) {
        throw logic_error {"No possible moves for Medium AI to choose from"};
    }

    // Find all checking moves from possibleMoves
    vector<Move> checkingMoves; // Stores checking moves in here

    for(auto m : possibleMoves) {
        // if move is to square that puts king into check, good
        vector<vector<bool>> states = g->getHasMovedStates();
        Board temp{g->getBoard()};
        temp.move(m);
        if(g->inCheck(temp, !isWhite)) {
            checkingMoves.emplace_back(m);
        }
        g->setHasMovedStates(states);
    }

    // If checkingMoves is non-empty, return a random move
    if(checkingMoves.size() > 0) {
        return checkingMoves[generateRandomInt(checkingMoves.size())];
    }

    // Find all capture moves from possibleMoves
    vector<Move> captureMoves; // Stores capture moves in here
    for(auto m : possibleMoves) {
        // if cell is not empty, then must contain opposing player piece
        if(!g->getBoard().getCell(m.newRow, m.newCol).getIsEmpty()) {
            captureMoves.emplace_back(m);
        } else {
            // handle en passent
            vector<vector<bool>> states = g->getHasMovedStates();
            Board temp{g->getBoard()};
            temp.move(m);
            if(temp.getWasEnPassent()) {

                captureMoves.emplace_back(m);
            }
            g->setHasMovedStates(states);
        }
    }
    // If captureMoves is non-empty, return a random move
    if(captureMoves.size() > 0) {
        return captureMoves[generateRandomInt(captureMoves.size())];
    }

    // If no capture or checking moves, then return a random move
    return possibleMoves[generateRandomInt(possibleMovesLength)];
}

Move HardAI::getNextMove()  {
    vector<Move> possibleMoves = g->generateValidMoves(isWhite);
    size_t possibleMovesLength = possibleMoves.size();

    if (possibleMovesLength == 0) {
        throw logic_error {"No possible moves for Hard AI to choose from"};
    }

    // Find all checking moves from possibleMoves
    vector<Move> checkingMoves; // Stores checking moves in here

    for(auto m : possibleMoves) {
        // if move is to square that puts king into check, good
        vector<vector<bool>> states = g->getHasMovedStates();
        Board temp{g->getBoard()};
        temp.move(m);
        if(g->inCheck(temp, !isWhite)) {
            checkingMoves.emplace_back(m);
        }
        g->setHasMovedStates(states);
    }

    
    // If checkingMoves is non-empty, return a random move
    if(checkingMoves.size() > 0) {
        return checkingMoves[generateRandomInt(checkingMoves.size())];
    }

    // Return a random move that avoids capture if possible
    
    // generate moves for other player
    // see if any moves capture one of AI's pieces, move to avoid
    vector<Move> possibleMovesAgainst = g->generateValidMoves(!isWhite);
    for(auto m : possibleMovesAgainst) {
        // see if any moves capture one of AI's pieces
        if(!g->getBoard().getCell(m.newRow, m.newCol).getIsEmpty()) {
            // find square to avoid
            for(auto myMove : possibleMoves) {
                if(myMove.oldRow == m.newRow && myMove.oldCol == m.newCol) {
                    return myMove;
                }
            }
        }
    }


    // Find all capture moves from possibleMoves
    vector<Move> captureMoves; // Stores capture moves in here
    // If captureMoves is non-empty, return a random move
    for(auto m : possibleMoves) {
        // if cell is not empty, then must contain opposing player piece
        if(!g->getBoard().getCell(m.newRow, m.newCol).getIsEmpty()) {
            captureMoves.emplace_back(m);
        } else {
            vector<vector<bool>> states = g->getHasMovedStates();
            Board temp{g->getBoard()};
            temp.move(m);
            if(temp.getWasEnPassent()) {
                captureMoves.emplace_back(m);
            }
            g->setHasMovedStates(states);
        }
    }
    // If captureMoves is non-empty, return a random move
    if(captureMoves.size() > 0) {
        return captureMoves[generateRandomInt(captureMoves.size())];
    }


    // If no capture or checking moves, then return a random move
    return possibleMoves[generateRandomInt(possibleMovesLength)];
}

Move BestAI::getNextMove() {
    vector<Move> possibleMoves = g->generateValidMoves(isWhite);
    // first, if possible, checkmate
    for(auto m : possibleMoves) {
        // if move is to square that puts king into checkmate, good
        vector<vector<bool>> states = g->getHasMovedStates();
        Board temp{g->getBoard()};
        temp.move(m);
        if(g->inCheckmate(temp, !isWhite)) {
            g->setHasMovedStates(states);
            return m;
        }
        g->setHasMovedStates(states);
    }

    vector<Move> possibleMovesAgainst = g->generateValidMoves(!isWhite);
    vector<Move> potentialLosses;
    for(auto m : possibleMovesAgainst) {
        // see if any moves capture one of AI's pieces
        if(!g->getBoard().getCell(m.newRow, m.newCol).getIsEmpty()) {
            // find square to avoid
            for(auto myMove : possibleMoves) {
                if(myMove.oldRow == m.newRow && myMove.oldCol == m.newCol) {
                    potentialLosses.emplace_back(m);
                }
            }
        }
    }

    vector<pair<Move, int>> pairs;
    // if other player can capture, focus on avoiding, 
    for(auto m : potentialLosses) {
        for(auto myMove : possibleMoves) {
            if(myMove.oldRow == m.newRow && myMove.oldCol == m.newCol) {
                // points gained for avoiding capture, points depend on piece type
                int score = 1;
                score += (int) g->getBoard().getCell(myMove.oldRow, myMove.oldCol).getPiece()->getPieceType() 
                    + determineMoveScore(myMove);
                pairs.emplace_back(make_pair(myMove, score));
            }
        }
    }

    // score each move depending on piece capture, checks give extra pts
    for(auto m : possibleMoves) {
        int score = 1;
        score += determineMoveScore(m);
        pairs.emplace_back(make_pair(m, score));
    }

    // find highest scoring move
    int max = -1;
    Move best;
    for(auto pair : pairs) {
        if(pair.second > max) {
            max = pair.second;
        }
    }

    // chance of having multiple high scoring moves, find all, return random
    vector<Move> bestMoves;
    for(auto pair : pairs) {
        if(pair.second == max) {
            bestMoves.emplace_back(pair.first);
        }
    }
    int bestMovesLength = bestMoves.size();
    return bestMoves[generateRandomInt(bestMovesLength)];
}

int BestAI::determineMoveScore(Move m) {
    int score = 0;

    // check if move captures

    // if not empty, piece is there
    if(!g->getBoard().getCell(m.newRow, m.newCol).getIsEmpty()) {
        score += 1 + (int) g->getBoard().getCell(m.newRow, m.newCol).getPiece()->getPieceType();
    }

    // check if move checks
    vector<vector<bool>> states = g->getHasMovedStates();
    Board temp{g->getBoard()};
    temp.move(m);
    if(g->inCheck(temp, !isWhite)) {
        // check has same value as queen
        score += 4;
    }
    g->setHasMovedStates(states);

    return score;
}

char EasyAI::handlePawnPromotion() {
    return 'Q';
}

char MediumAI::handlePawnPromotion() {
    return 'Q';
}

char HardAI::handlePawnPromotion() {
    // maybe make it check for checks?
    return 'Q';
}

char BestAI::handlePawnPromotion() {
    return 'Q';
}

