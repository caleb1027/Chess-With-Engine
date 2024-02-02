#include "bitboard.h"
using std::cout;
using std::endl;

// TODO: refactor methods lol

// use tables to generate non-sliding moves
// may need to check for check in a bitboard, figure out how to do that
// 12 bitboards, one for each piece type, one for each color
// 1 bitboard for all white pieces, one for all black pieces
// 1 bitboard for occupied squares

const int UP_LEFT = -9;
const int UP_RIGHT = -7;
const int DOWN_LEFT = 7;
const int DOWN_RIGHT = 9;
const int UP = -8;
const int DOWN = 8;
const int LEFT = -1;
const int RIGHT = 1;


Bitboards::Bitboards():
    occupiedBlack(0ULL), occupiedWhite(0ULL),
    validEnPassentDestinations(0ULL), firstMove(0ULL), 
    whiteBoards(6, 0ULL), blackBoards(6, 0ULL), whiteToMove(true) {
    // initial positions for white
    for(int i = 0; i < 8; ++i) {
        set_bit(whiteBoards[Pawn], (a2 + i));
    }
    set_bit(whiteBoards[Rook], a1);
    set_bit(whiteBoards[Rook], h1);
    set_bit(whiteBoards[Knight], b1);
    set_bit(whiteBoards[Knight], g1);
    set_bit(whiteBoards[Bishop], c1);
    set_bit(whiteBoards[Bishop], f1);
    set_bit(whiteBoards[Queen], d1);
    set_bit(whiteBoards[King], e1);

    // initial positions for black
    for(int i = 0; i < 8; ++i) {
        set_bit(blackBoards[Pawn], (a7 + i));
    }
    set_bit(blackBoards[Rook], a8);
    set_bit(blackBoards[Rook], h8);
    set_bit(blackBoards[Knight], b8);
    set_bit(blackBoards[Knight], g8);
    set_bit(blackBoards[Bishop], c8);
    set_bit(blackBoards[Bishop], f8);
    set_bit(blackBoards[Queen], d8);
    set_bit(blackBoards[King], e8);

    // set first move and occupied boards
    for(int i = 0; i < 6; ++i) {
        occupiedWhite |= whiteBoards[i];
        occupiedBlack |= blackBoards[i];
    }
    firstMove = occupiedWhite | occupiedBlack;
}

void Bitboards::printAsBoard() {
    char board[8][8];
    std::map<int, char> pieceMap = {{Pawn, 'P'}, {Knight, 'N'}, {Bishop, 'B'}, {Rook, 'R'}, {Queen, 'Q'}, {King, 'K'}};
    // init board
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            board[row][col] = (row % 2) ^ (col % 2) ? '_' : ' ';
        }
    }
    // get white pieces
    for(int i = 0; i < 6; ++i) {
        bitboard pieceBoard = whiteBoards[i];
        while(pieceBoard) {
            int square = __builtin_ctzll(pieceBoard);
            board[square / 8][square % 8] = pieceMap[i];
            pop_bit(pieceBoard, square);
        }
    }
    // get black pieces
    for(int i = 0; i < 6; ++i) {
        bitboard pieceBoard = blackBoards[i];
        while(pieceBoard) {
            int square = __builtin_ctzll(pieceBoard);
            // adjust to lowercase
            board[square / 8][square % 8] = pieceMap[i] + ('a'-'A');
            pop_bit(pieceBoard, square);
        }
    }
    // print board
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            cout << board[row][col];
        }
        cout << endl;
    }
}

void Bitboards::printBitboard(bitboard b) {
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            cout << (get_bit(b, (row * 8 + col)) ? 1 : 0) << " ";
        }
        cout << endl;
    }
}

bool Bitboards::isCapture(FastMove m) {
    if(m.isWhite) {
        return get_bit(occupiedBlack, m.to);
    } else {
        return get_bit(occupiedWhite, m.to);
    }
}

bool Bitboards::isOccupied(bool isWhite, int to) {
    if(isWhite) {
        return get_bit(occupiedWhite, to);
    } else {
        return get_bit(occupiedBlack, to);
    }
}

vector<FastMove> Bitboards::getValidMovesPawn(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOwn;
    bitboard occupiedByOther;
    if(getWhiteMoves)  {
        positions = whiteBoards[Pawn];
        occupiedByOwn = occupiedWhite;
        occupiedByOther = occupiedBlack;
    } else {
        positions = blackBoards[Pawn];
        occupiedByOwn = occupiedBlack;
        occupiedByOther = occupiedWhite;
    }   
    // check double move
    // get all pawns of one color that haven't moved
    bitboard possibleDoubleMoves = positions & firstMove;
    while(possibleDoubleMoves) {
        int square = __builtin_ctzll(possibleDoubleMoves);
        // check if pawn is white or black
        if(getWhiteMoves) {
            // check if square in front of pawn is occupied
            if(!get_bit(occupiedByOwn, (square - 8))) {
                // check if square 2 in front of pawn is occupied
                if(!get_bit(occupiedByOwn, (square - 16))) {
                    moves.push_back({square, (square - 16), getWhiteMoves, Pawn});
                }
            }
        }
        pop_bit(possibleDoubleMoves, square);   
    }

    while(positions) {
        // get bit position of knight by counting leading zeros
        int square = __builtin_ctzll(positions);
        bitboard movesMask = pawnMoveMasks[getWhiteMoves][square];
        // remove occupied squares from move mask
        movesMask &= ~occupiedByOwn;
        // add valid moves to vector
        while(movesMask) {
            int to = __builtin_ctzll(movesMask);
            // check if moving forward, else capture
            if(abs(square - to) == 8 && !get_bit(occupiedByOther, to)) {
                moves.push_back({square, to, getWhiteMoves, Pawn});
            } else {
                if(isCapture({square, to, getWhiteMoves, Pawn})) {
                    moves.push_back({square, to, getWhiteMoves, Pawn});
                } else {
                    // check en passent
                    if(movesMask & validEnPassentDestinations) moves.push_back({square, __builtin_ctzll(movesMask & validEnPassentDestinations), getWhiteMoves, Pawn});
                }
            }
            pop_bit(movesMask, to);
        }
        pop_bit(positions, square);
    }

    // TODO: need to account for promotion

    return moves;
}

vector<FastMove> Bitboards::getValidMovesKing(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOwn;
    if(getWhiteMoves)  {
        positions = whiteBoards[King];
        occupiedByOwn = occupiedWhite;
    } else {
        positions = blackBoards[King];
        occupiedByOwn = occupiedBlack;
    }   
    // only have 1 king per side, no need for loop
    int square = __builtin_ctzll(positions);
    bitboard movesMask = kingMoveMasks[square];
    // remove occupied squares from move mask
    movesMask &= ~occupiedByOwn;
    // add valid moves to vector
    while(movesMask) {
        int to = __builtin_ctzll(movesMask);
        moves.push_back({square, to, getWhiteMoves, King});
        pop_bit(movesMask, to);
    }

    // account for castling
    return moves;
}

vector<FastMove> Bitboards::getValidMovesKnight(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOwn;
    // get position of knights and occupied squares of one color
    if(getWhiteMoves) {
        positions = whiteBoards[Knight];
        occupiedByOwn = occupiedWhite;
    } else {
        positions = blackBoards[Knight];
        occupiedByOwn = occupiedBlack;
    }
    // for each knight, get move table, and then calculate valid moves
    while(positions) {
        // get bit position of knight by counting leading zeros
        int square = __builtin_ctzll(positions);
        bitboard movesMask = knightMoveMasks[square];
        // remove occupied squares from move mask
        movesMask &= ~occupiedByOwn;
        // add valid moves to vector
        while(movesMask) {
            int to = __builtin_ctzll(movesMask);
            moves.push_back({square, to, getWhiteMoves, Knight});
            pop_bit(movesMask, to);
        }
        pop_bit(positions, square);
    }
    return moves;
}

vector<FastMove> Bitboards::getValidMovesBishop(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOther;
    if(getWhiteMoves) {
        positions = whiteBoards[Bishop];
        occupiedByOther = occupiedWhite;
    } else {
        positions = blackBoards[Bishop];
        occupiedByOther = occupiedBlack;
    }
    int directions[4] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
    
    // concat vectors of moves in each direction
    for(int dir : directions) {
        vector<FastMove> temp = getMovesDirection(positions, occupiedByOther, getWhiteMoves, dir);
        std::move(temp.begin(), temp.end(), std::back_inserter(moves)); 
    }
    return moves;
}

vector<FastMove> Bitboards::getValidMovesRook(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOther;
    if(getWhiteMoves) {
        positions = whiteBoards[Rook];
        occupiedByOther = occupiedWhite;
    } else {
        positions = blackBoards[Rook];
        occupiedByOther = occupiedBlack;
    }
    // concat vectors of moves in each direction
    int directions[4] = {UP, DOWN, LEFT, RIGHT};
    for(int dir : directions) {
        vector<FastMove> temp = getMovesDirection(positions, occupiedByOther, getWhiteMoves, dir);
        std::move(temp.begin(), temp.end(), std::back_inserter(moves)); 
    }
    return moves;
}

vector<FastMove> Bitboards::getValidMovesQueen(bool getWhiteMoves) {
    vector<FastMove> moves;
    bitboard positions;
    bitboard occupiedByOther;
    if(getWhiteMoves) {
        positions = whiteBoards[Queen];
        occupiedByOther = occupiedWhite;
    } else {
        positions = blackBoards[Queen];
        occupiedByOther = occupiedBlack;
    }
    // concat vectors of moves in each direction
    int directions[8] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT, UP, DOWN, LEFT, RIGHT};
    for(int dir : directions) {
        vector<FastMove> temp = getMovesDirection(positions, occupiedByOther, getWhiteMoves, dir);
        std::move(temp.begin(), temp.end(), std::back_inserter(moves)); 
    }
    return moves;
}

bool Bitboards::inCheck(bool checkWhite) {
    bitboard kingPosn;
    vector<bitboard> opponentBoards;
    if(checkWhite) {
        kingPosn = __builtin_ctzll(whiteBoards[King]);
        opponentBoards = blackBoards;
    } else {
        kingPosn = blackBoards[King];
        opponentBoards = whiteBoards;
    }
    // check horses and pawns to see if they can capture king
    if(knightMoveMasks[kingPosn] & opponentBoards[Knight]) return true;
    // -1 to adjust for direction
    if(get_bit(opponentBoards[Pawn], (kingPosn + (7 * (-1 * checkWhite)))) 
    || get_bit(opponentBoards[Pawn], (kingPosn + (9 * (-1 * checkWhite))))) {
        return true;
    }
    // then, go along in each direction to see if you find an enemy
    // queen, rook, or bishop before your own piece
    int rookDirections[] = {UP, DOWN, LEFT, RIGHT};
    for(int dir : rookDirections) {
        int to = kingPosn + dir;
        while(to >= 0 && to < 64) {
            if(get_bit((opponentBoards[Queen] | opponentBoards[Rook]), to)) {
                return true;
            }
            if(get_bit((occupiedBlack | occupiedWhite), to)) break;
            to += dir;
        }
    }

    int bishopDirections[] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
    for(int dir : bishopDirections) {
        int to = kingPosn + dir;
        while(to >= 0 && to < 64) {
            if(get_bit((opponentBoards[Queen] | opponentBoards[Bishop]), to)) {
                return true;
            }
            if(get_bit((occupiedBlack | occupiedWhite), to)) break;
            to += dir;
        }
    }

    return false;
}

bool Bitboards::inCheckmate(bool checkWhite) {
    // check if in check
    if(!inCheck(checkWhite)) return false;
    // check for valid moves
    if(!getValidMovesKing(checkWhite).empty()) return false;
    return true;
}

vector<FastMove> Bitboards::getMovesDirection(bitboard positions, const bitboard &occupiedByOther, bool getWhiteMoves, int direction) {
    vector<FastMove> moves;
    while(positions) {
        int square = __builtin_ctzll(positions);
        int to = square + direction;
        while(to >= 0 && to < 64) {
            if(isOccupied(getWhiteMoves, to)) break;
            if(get_bit(occupiedByOther, to)) {
                moves.push_back({square, to, getWhiteMoves, Bishop});
                break;
            }
            moves.push_back({square, to, getWhiteMoves, Bishop});
            square = to;
        }
        pop_bit(positions, square);
    }
    return moves;
}

vector<FastMove> Bitboards::getValidMoves(bool getWhiteMoves) {
    vector<FastMove> moves;
    // get all valid moves for each piece type
    vector<FastMove> pawnMoves = getValidMovesPawn(getWhiteMoves);
    vector<FastMove> knightMoves = getValidMovesKnight(getWhiteMoves);
    vector<FastMove> bishopMoves = getValidMovesBishop(getWhiteMoves);
    vector<FastMove> rookMoves = getValidMovesRook(getWhiteMoves);
    vector<FastMove> queenMoves = getValidMovesQueen(getWhiteMoves);
    vector<FastMove> kingMoves = getValidMovesKing(getWhiteMoves);
    // concat all vectors
    moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
    moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), queenMoves.begin(), queenMoves.end());
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());

    for(size_t i = 0; i < moves.size(); ++i) {
        // check for moving into check
        Bitboards temp{*this};
        temp.movePiece(moves[i]);
        if(temp.inCheck(getWhiteMoves)) {
            moves.erase(moves.begin() + i);
        }
    }
    return moves;
}

// generates move tables for non-sliding pieces, not super worried about efficiency here
void Bitboards::generateMoveTables() {
    // generate white pawn move masks 
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            bitboard mask = 0ULL;
            int square = row * 8 + col;
            if(row - 1 >= 0) {
                // forward
                set_bit(mask, (square - 8));
                // sideways
                if(col - 1 >= 0) {
                    set_bit(mask, (square - 9));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square - 7));
                }
            }
            pawnMoveMasks[1][square] = mask;
        }
    }

    // generate black pawn move masks
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            bitboard mask = 0ULL;
            int square = row * 8 + col;
            if(row + 1 < 8) {
                // forward
                set_bit(mask, (square + 8));
                // sideways
                if(col - 1 >= 0) {
                    set_bit(mask, (square + 7));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square + 9));
                }
            }
            pawnMoveMasks[0][square] = mask;
        }
    }

    // generate king move masks
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            bitboard mask = 0ULL;
            int square = row * 8 + col;

            if(row - 1 >= 0) {
                set_bit(mask, (square - 8));
                if(col - 1 >= 0) {
                    set_bit(mask, (square - 9));
                     set_bit(mask, (square - 1));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square - 7));
                    set_bit(mask, (square + 1));
                }
            }
            if(row + 1 < 8) {
                set_bit(mask, (square + 8));
                if(col - 1 >= 0) {
                    set_bit(mask, (square + 7));
                    set_bit(mask, (square - 1));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square + 9));
                    set_bit(mask, (square + 1));
                }
            }
            kingMoveMasks[square] = mask;
        }
    }

    // generate knight move tables
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            bitboard mask = 0ULL;
            int square = row * 8 + col;

            if(row - 2 >= 0) {
                if(col - 1 >= 0) {
                    set_bit(mask, (square - 17));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square - 15));
                }
            }
            if(row + 2 < 8) {
                if(col - 1 >= 0) {
                    set_bit(mask, (square + 15));
                }
                if(col + 1 < 8) {
                    set_bit(mask, (square + 17));
                }
            }
            if(col - 2 >= 0) {
                if(row - 1 >= 0) {
                    set_bit(mask, (square - 10));
                }
                if(row + 1 < 8) {
                    set_bit(mask, (square + 6));
                }
            } 
            if(col + 2 < 8) {
                if(row - 1 >= 0) {
                    set_bit(mask, (square - 6));
                }
                if(row + 1 < 8) {
                    set_bit(mask, (square + 10));
                }
            }
            knightMoveMasks[square] = mask;
        }
    }
}

void Bitboards::movePiece(FastMove m) {
    // update first move
    pop_bit(firstMove, m.from);
    // update occupied squares
    if(m.isWhite) {
        pop_bit(occupiedWhite, m.from);
        set_bit(occupiedWhite, m.to);
    } else {
        pop_bit(occupiedBlack, m.from);
        set_bit(occupiedBlack, m.to);
    }
    // update piece boards
    pop_bit(whiteBoards[m.pieceType], m.from);
    set_bit(whiteBoards[m.pieceType], m.to);
    // update en passent
    if(m.pieceType == Pawn && abs(m.from - m.to) == 16) {
        set_bit(validEnPassentDestinations, m.to);
    } else {
        validEnPassentDestinations = 0ULL;
    }

    // check capture
    if(isCapture(m)) {
        // update piece boards
        if(m.isWhite) {
            // all bits should be off except 1
            for(int i = 0; i < 6; ++i) {
                pop_bit(blackBoards[i], m.to);
            }
        } else {
           for(int i = 0; i < 6; ++i) {
                pop_bit(blackBoards[i], m.to);
            }
        }
    }

    // check castling
    if(m.pieceType == King && abs(m.from - m.to) == 2) {
        // check if king side or queen side
        if(m.isWhite) {
            if(m.to > m.from) {
                // king side
                // move rook
                pop_bit(whiteBoards[Rook], h1);
                set_bit(whiteBoards[Rook], f1);
            } else {
                // queen side
                // move rook
                pop_bit(whiteBoards[Rook], a1);
                set_bit(whiteBoards[Rook], d1);
            }
        } else {
            if(m.to > m.from) {
                // king side
                // move rook
                pop_bit(blackBoards[Rook], h8);
                set_bit(blackBoards[Rook], f8);
            } else {
                // queen side
                // move rook
                pop_bit(blackBoards[Rook], a8);
                set_bit(blackBoards[Rook], d8);
            }
        }
        
    }
    // update occupied squares on bitboards
    updateNonPieceBoards();
    // give move to other player
    whiteToMove = !whiteToMove;
}

void Bitboards::updateNonPieceBoards() {
    // update occupied squares
    occupiedWhite = 0ULL;
    occupiedBlack = 0ULL;
    for(int i = 0; i < 6; ++i) {
        occupiedWhite |= whiteBoards[i];
        occupiedBlack |= blackBoards[i];
    }

}


int main() {
    Bitboards::generateMoveTables();
    Bitboards test{};
    test.printAsBoard();
    vector<FastMove> moves = test.getValidMoves(true);
    for(FastMove m : moves) {
        cout << m.from << " " << m.to << endl;
    }
    cout << moves.size() << endl;
    return 0;
}