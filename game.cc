#include "game.h"
#include "players/humanplayer.h"
#include "players/AIs.h"

using namespace std;

Game::Game(bool useGUI):
board{Board()}, isSetup{false}, whiteToMove{true}, whiteScore{0}, blackScore{0}, 
views{vector<unique_ptr<View>>()}, moveList{vector<Move>()}, p1{nullptr}, p2{nullptr}
{
    // create text view
    views.emplace_back(std::make_unique<TextView>(board));
    // create gui view
    if(useGUI) views.emplace_back(std::make_unique<GraphicsView>(board));
}

Game::~Game() {}

void Game::updateView() {
    for(size_t i = 0; i < views.size(); ++i) {
        views[i]->setBoard(board);
        views[i]->display();
    }
}

void Game::updateMoveList(Move m) {
    moveList.emplace_back(m);
}

void Game::play() {
    string cmd;
    bool playing = false;
    while(!cin.eof()) {
        cin >> cmd;
        if(cmd == "game") {
            if(!isSetup) {
                board.initBoard();
                board.initPieces();
                board.setGameState(GameState::Playing);
                whiteToMove = true;
            }
            string player;

            // read white player
            cin >> player;
            if(player == "human") {
                p1 = make_unique<HumanPlayer>(true, this);
            } else if(player == "computer1"){
                p1 = make_unique<EasyAI>(true, this);
            } else if(player == "computer2"){
                p1 = make_unique<MediumAI>(true, this);
            } else if(player == "computer3"){
                p1 = make_unique<HardAI>(true, this);
            } else if(player == "computer4"){
                p1 = make_unique<BestAI>(true, this);
            } else {
                cout << "Error." << endl;
                continue;
            }
            
            // read black player
            cin >> player;
            if(player == "human") {
                p2 = std::make_unique<HumanPlayer>(false, this);
            } else if(player == "computer1"){
                p2 = make_unique<EasyAI>(false, this);
            } else if(player == "computer2"){
                p2 = make_unique<MediumAI>(false, this);
            } else if(player == "computer3"){
                p2 = make_unique<HardAI>(false, this);
            } else if(player == "computer4"){
                p2 = make_unique<BestAI>(false, this);
            } else {
                cout << "Error." << endl;
                continue;
            }
            playing = true;
            if (inStalemate(!whiteToMove) || inStalemate(whiteToMove)) {
                board.setGameState(GameState::Stalemate);
                whiteScore += 0.5;
                blackScore += 0.5;
                playing = false;
            }
            updateView();
            handleGameEnd(playing);
        } else if(cmd == "resign") {
            if(whiteToMove) {
                board.setGameState(GameState::WhiteResigned);
                blackScore += 1;
            } else {
                board.setGameState(GameState::BlackResigned);
                whiteScore += 1;
            }
            updateView();
            playing = false;
            handleGameEnd(playing);
        } else if(cmd == "move") {
            if(!playing) {
                cout << "Error, can't make move as no game is in progress" << endl;
                continue;
            }
            Move m;
            if (whiteToMove) {
                // try catch?
                m = p1->getNextMove();
            } else {
                m = p2->getNextMove();
            }
            if(cin.eof()) {
                break;
            }
            if(!validateMove(board, m, true)) {
                cout << "Invalid move" << endl;
                cin.clear();
                continue;
            }
            // if move is valid, check if move puts current player in check (ie. moving pinned piece)
            vector<vector<bool>> hasMovedStates = getHasMovedStates();
            Board temp{board};
            temp.move(m);
            if(inCheck(temp, whiteToMove)) {
                cout << "Invalid move, move puts you in check." << endl;
                setHasMovedStates(hasMovedStates);
                continue;
            }
            // temp updates states of main board pieces, revert to what they were previously
            setHasMovedStates(hasMovedStates);

            board.move(m);
            updateMoveList(m);
            
            // handle pawn promotion
            if(board.getCell(m.newRow, m.newCol).getPiece()->getPieceType() == PieceType::Pawn
            && ((m.newRow == 7 && board.getCell(m.newRow, m.newCol).getPiece()->getIsWhite())
            || (m.newRow == 0 && !board.getCell(m.newRow, m.newCol).getPiece()->getIsWhite()))) {
                char c;
                while(!cin.eof()) {
                    if(whiteToMove) {
                        c = p1->handlePawnPromotion();
                    } else {
                        c = p2->handlePawnPromotion();
                    }
                    if(cin.eof()) break;
                    if(cin.fail()) {
                        cout << "Invalid, please try again." << endl;
                        cin.clear();
                    }
                    if(!board.handlePawnPromotion(board.getCell(m.newRow, m.newCol), c)) {
                        cout << "Invalid type, please try again." << endl;
                    } else {
                        break;
                    }
                    
                }
                if(cin.eof()) break;
            }

            // if white just moved, check if black is in check
            board.setGameState(GameState::Playing);
            if (inStalemate(!whiteToMove)) {
                board.setGameState(GameState::Stalemate);
                whiteScore += 0.5;
                blackScore += 0.5;
                playing = false;
            } else if (inCheckmate(board, false)) {
                board.setGameState(GameState::BlackInCheckmate);
                whiteScore += 1;
                playing = false;
            } else if (inCheck(board, false)) {
                board.setGameState(GameState::BlackInCheck);
            } else if (inCheckmate(board, true)) {
                    board.setGameState(GameState::WhiteInCheckmate);
                    blackScore += 1;
                    playing = false;
            } else if (inCheck(board, true)) {
                board.setGameState(GameState::WhiteInCheck);
            }
            

            whiteToMove = !whiteToMove;
            updateView();
            handleGameEnd(playing);
        } else if(cmd == "setup") {
            if(playing) {
                cout << "Error, cannot enter setup while playing." << endl;
                continue;
            }
            isSetup = true;
            board.initBoard();
            // MUST CHECK FOR # OF KINGS, PAWN POSN, CHECKS
            while(!cin.eof()) {
                string cmd;
                cin >> cmd;
                if(cmd == "done") {
                    if (setupValid()) {
                        break;
                    }
                    cout << "Invalid setup." << endl;
                    continue;
                };

                if(cmd == "+") {
                    setupAdd();
                } else if(cmd == "-") {
                    string square;
                    cin >> square;
                    if(cin.fail()) {
                        cout << "Invalid input." << endl;
                        continue;
                    }
                    int x;
                    int y;
                    x = square[0] - 'a';
                    y = square[1] - '1';

                    if((x < 0 || x > 7) || (y < 0 || y > 7)) {
                        cout << "Invalid square." << endl;
                        continue;
                    }
                    if(!board.getCell(y, x).getIsEmpty()) {
                        board.removePiece(y, x); // removePiece() also mixes up y and x
                    } else {
                        cout << "Invalid, square is empty." << endl;
                    }
                } else if(cmd == "=") {
                    string colour;
                    cin >> colour;
                    if(colour == "white") {
                        whiteToMove = true;
                    } else if(colour == "black") {
                        whiteToMove = false;
                    } else {
                        cout << "Invalid colour." << endl;
                    }
                } else if (cin.eof()) {
                    break;
                } else {
                    cout << "Invalid Command." << endl;
                }
                updateView();
            }
        } else if(!cin.eof()) {
            cout << "Invalid Command." << endl;
        }
    }
    // print score
    cout << "Final Score:" << endl;
    cout << "White: " << whiteScore << endl;
    cout << "Black: " << blackScore << endl;
}

void Game::setupAdd() {
    char type;
    string square;

    PieceType pt;
    bool white = false;

    cin >> type;
    if(type == 'k' || type == 'K') {
        pt = PieceType::King;
    } else if(type == 'q' || type == 'Q') {
        pt = PieceType::Queen;
    } else if(type == 'r' || type == 'R') {
        pt = PieceType::Rook;
    } else if(type == 'b' || type == 'B') {
        pt = PieceType::Bishop;
    } else if(type == 'n' || type == 'N') {
        pt = PieceType::Knight;
    } else if(type == 'p' || type == 'P') {
        pt = PieceType::Pawn;
    } else {
        return;
        // invalid input
    }

    if(type >= 'A' && type <= 'Z') {
        white = true;
    }

    cin >> square;
    int x;
    int y;
    x = square[0] - 'a';
    y = square[1] - '1';
    if((x < 0 || x > 7) || (y < 0 || y > 7)) {
        // invalid
        return;
    }

    // add piece
    board.addPiece(y, x, pt, white); // i think our addPiece mixes up row and col
}

bool Game::setupValid() {
    // check for one of each king
    int whiteKingCounter = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!board.getCell(row, col).getIsEmpty() 
            && board.getCell(row, col).getPiece()->getPieceType() == PieceType::King 
            && board.getCell(row, col).getPiece()->getIsWhite() == true)
            {
                whiteKingCounter++;
            }
        }
    }
    if (whiteKingCounter != 1) {
        return false;
    }

    int blackKingCounter = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!board.getCell(row, col).getIsEmpty() 
            && board.getCell(row, col).getPiece()->getPieceType() == PieceType::King 
            && board.getCell(row, col).getPiece()->getIsWhite() == false) {
                blackKingCounter++;
            }
        }
    }
    if (blackKingCounter != 1) {
        return false;
    }

    // check for no pawns on first row
    int row = 0;
    for (int col = 0; col < 8; ++col) {
        if (!board.getCell(row, col).getIsEmpty() 
        && board.getCell(row, col).getPiece()->getPieceType() == PieceType::Pawn) {
            return false;
        }
    }

    // no pawns on last row
    row = 7;
    for (int col = 0; col < 8; ++col) {
        if (!board.getCell(row, col).getIsEmpty() && board.getCell(row, col).getPiece()->getPieceType() == PieceType::Pawn) {
            return false;
        }
    }

    // check kings are not in check
    if (inCheck(board, true) || inCheck(board, false)) {
        return false;
    }

    return true;
}

// THIS FUNCTION DOESN'T CHECK FOR CHECK!
bool Game::validateMove(Board b, Move m, bool checkColour) {

    bool valid = true;
    // check if move is out of bounds
    if((m.oldCol < 0 || m.oldCol > 7) || (m.oldRow < 0 || m.oldRow > 7) || (m.newCol < 0 || m.newCol > 7) || (m.newRow < 0 || m.newRow > 7)) {
        return false;
    }
    // check if move is to same square
    if(m.oldCol == m.newCol && m.oldRow == m.newRow) {
        return false;
    }

    // check if square is empty
    if(b.getCell(m.oldRow, m.oldCol).getIsEmpty()) {
        return false;
    }
    // check if piece belongs to player
    if(checkColour && b.getCell(m.oldRow, m.oldCol).getPiece()->getIsWhite() != whiteToMove) {
        return false;
    }

    // check if move is valid for piece type (pawn is different)
    if(!b.getCell(m.oldRow, m.oldCol).getPiece()->validateMove(m)) {
        return false;
    }

    // check if moving onto occupied square
    if(!b.getCell(m.newRow, m.newCol).getIsEmpty() && 
    b.getCell(m.newRow, m.newCol).getPiece()->getIsWhite() == b.getCell(m.oldRow, m.oldCol).getPiece()->getIsWhite()) {
        // account for castling, allows for player to move king to rook's square to castle
        if(!b.getCell(m.oldRow, m.oldCol).getPiece()->getHasMoved() 
        && !b.getCell(m.newRow, m.newCol).getPiece()->getHasMoved()
        && b.getCell(m.oldRow, m.oldCol).getPiece()->getPieceType() == PieceType::King 
        && b.getCell(m.newRow, m.newCol).getPiece()->getPieceType() == PieceType::Rook) {
            valid = true;
        } else {
            return false;
        }
    }

    PieceType p = b.getCell(m.oldRow, m.oldCol).getPiece()->getPieceType();
    // check pawn
    if(b.getCell(m.oldRow, m.oldCol).getPiece()->getPieceType() == PieceType::Pawn) {

        // capture logic
        bool capture = false;
        if(abs(m.newCol - m.oldCol) == 1 && b.getCell(m.newRow, m.newCol).getIsEmpty()) {
            valid = false;
        }
        if(m.oldCol == m.newCol && !b.getCell(m.newRow, m.newCol).getIsEmpty()) {
            valid = false;
        }
        if(abs(m.newCol - m.oldCol) == 1 && !b.getCell(m.newRow, m.newCol).getIsEmpty()) {
            valid = true;
            capture = true;
        }
        // ensure moving twice doesn't jump over piece
        if(abs(m.newRow - m.oldRow) == 2) {
            if(m.newRow > m.oldRow && !b.getCell(m.oldRow + 1, m.oldCol).getIsEmpty()) {
                return false;
            } else if(m.newRow < m.oldRow && !b.getCell(m.oldRow - 1, m.oldCol).getIsEmpty()) {
                return false;
            }
        }

        // en passent, only legal after the advance, check move list's last item
        if(moveList.size() > 0 && !capture
        && abs(m.newCol - m.oldCol) == 1 && abs(m.newRow - m.oldRow) == 1) {
            Move prev = moveList.back();
            // check if last move was a 2 step pawn move (probably gonna create an edge case in terms of check)
            if(abs(prev.newRow - prev.oldRow) == 2 && b.getCell(prev.newRow, prev.newCol).getPiece()->getPieceType() == PieceType::Pawn) {
                // if opposing pawn is to side, same row, allow
                if(abs(prev.newCol - m.oldCol) == 1 
                    && prev.newRow == m.oldRow 
                    && prev.newCol == m.newCol
                    && b.getCell(prev.newRow, prev.newCol).getPiece()->getIsWhite() != b.getCell(m.oldRow, m.oldCol).getPiece()->getIsWhite()) {
                    board.setEnPassent(true);
                    valid = true;
                }                
            }
        } else {
            board.setEnPassent(false);
        }
    } else if(p == PieceType::King) {
        // castling logic (don't need to worry after setup)
        // check if player is trying to castle (can't castle out of check)
        if((abs(m.newCol - m.oldCol) == 2)) {
            // undefined on setup
            if(isSetup) return false;
            // if moving rows, invalid
            if(m.newRow != m.oldRow) return false;

            // check if king has moved
            if(b.getCell(m.oldRow, m.oldCol).getPiece()->getHasMoved()) {
                return false;
            }
            // determine direction of castle
            bool farCastle;
            // castle left
            if(m.oldCol > m.newCol) {
                farCastle = true;
            } else {
                farCastle = false;
            }
            // check if piece is rook / rook's moved
            int rookCol = (farCastle ? 0 : 7);

            if(b.getCell(m.oldRow, rookCol).getIsEmpty() || b.getCell(m.oldRow, rookCol).getPiece()->getPieceType() != PieceType::Rook
                || b.getCell(m.oldRow, rookCol).getPiece()->getHasMoved()) {
                return false;
            }
            // check if any pieces in the way
            for(int i = min(m.oldCol, rookCol) + 1; i < max(m.oldCol, rookCol); ++i) {
                if(i != m.newCol && !b.getCell(m.oldRow, i).getIsEmpty()) {
                    return false;
                }
            }

            // check if in check on middle position
            if(farCastle) {
                vector<vector<bool>> states = getHasMovedStates();
                Board temp{board};
                Move mid {m.oldCol, m.oldRow, m.oldCol - 1, m.newRow, m.whiteMove};
                temp.move(mid);
                if(inCheck(temp, m.whiteMove)) {
                    setHasMovedStates(states);
                    return false;
                }
                setHasMovedStates(states);
            } else {
                vector<vector<bool>> states = getHasMovedStates();
                Board temp{board};
                Move mid {m.oldCol, m.oldRow, m.oldCol + 1, m.newRow, m.whiteMove};
                temp.move(mid);
                if(inCheck(temp, m.whiteMove)) {
                    setHasMovedStates(states);
                    return false;
                }
                setHasMovedStates(states);

            }

            // can't castle out of check
            if(inCheck(b, b.getCell(m.oldRow, rookCol).getPiece()->getIsWhite())) {
                return false;
            }
            // let board handle move
            board.setCastle(true);
        } else {
            board.setCastle(false);
        }
    } else if(p != PieceType::Knight) {
        // knight move checking is done by all of previous conditions
        // check direction
        // horizontal
        if(m.oldCol == m.newCol) {
            for(int i = min(m.oldRow, m.newRow) + 1; i < max(m.oldRow, m.newRow); ++i) {
                if(i != m.newRow && !b.getCell(i, m.oldCol).getIsEmpty()) {
                    return false;
                }
            }
        }
        // vertical
        else if(m.oldRow == m.newRow) {
            for(int i = min(m.oldCol, m.newCol) + 1; i < max(m.oldCol, m.newCol); ++i) {
                if(i != m.newCol && !b.getCell(m.oldRow, i).getIsEmpty()) {
                    return false;
                }
            }
        } 
        // diagonal
        else {
            // determine horizontal direction and vertical direction
            int colDir = ((m.newCol - m.oldCol > 0) ? 1 : -1);
            int rowDir = ((m.newRow - m.oldRow > 0) ? 1 : -1);

            int currentRow = m.oldRow + rowDir;
            int currentCol = m.oldCol + colDir;
            while(currentRow != m.newRow) {
                if(!b.getCell(currentRow, currentCol).getIsEmpty()) {
                    return false;
                }
                currentRow += rowDir;
                currentCol += colDir;
            }
            valid = true;
        }
    }
    return valid;
}

vector<Move> Game::generateValidMoves(bool whiteTurn) {
    vector<Move> validMoves;
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(!board.getCell(i, j).getIsEmpty() && board.getCell(i, j).getPiece()->getIsWhite() == whiteTurn){
                for(int k = 0; k < 8; ++k) {
                    for(int l = 0; l < 8; ++l) {
                        Move m{j, i, l, k, whiteTurn};
                        if(validateMove(board, m, false)) {
                            vector<vector<bool>> states = getHasMovedStates();
                            Board temp{board};
                            // cout<< i << j << " to " << k << l << endl;
                            temp.move(m);
                            // ensures move doesn't put player in check
                            if(!inCheck(temp, whiteTurn)) {
                                validMoves.emplace_back(m);
                            }
                            setHasMovedStates(states);
                        } 
                    }
                }
            }
        }
    }
    return validMoves;
}

vector<Move> Game::generateValidMoves(Board b, bool whiteTurn) {
    vector<Move> validMoves;
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if(!b.getCell(i, j).getIsEmpty() && b.getCell(i, j).getPiece()->getIsWhite() == whiteTurn){
                for(int k = 0; k < 8; ++k) {
                    for(int l = 0; l < 8; ++l) {
                        Move m{j, i, l, k, whiteTurn};
                        if(validateMove(b, m, false)) {
                            vector<vector<bool>> states = getHasMovedStates(b);
                            Board temp{b};
                            // cout<< i << j << " to " << k << l << endl;
                            temp.move(m);
                            // ensures move doesn't put player in check
                            if(!inCheck(temp, whiteTurn)) {
                                validMoves.emplace_back(m);
                            }
                            setHasMovedStates(b, states);
                        } 
                    }
                }
            }
        }
    }
    return validMoves;
}

// check board state for check
bool Game::inCheck(Board b, bool checkWhite) {

    // find king of specified color
    vector<int> kingPosn = findKing(b, checkWhite);
    int kingRow = kingPosn[0];
    int kingCol = kingPosn[1];
    // loop through all pieces of other color, see if it's possible to capture king
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            // validate move only if piece doesn't belong to player that's potentially in check
            if(!b.getCell(row, col).getIsEmpty() && b.getCell(row, col).getPiece()->getIsWhite() != checkWhite) {
                Move m{col, row, kingCol, kingRow, !checkWhite};
                if(validateMove(b, m, false)) {
                    return true;
                }
            }   
        }   
    }
    return false;
}

bool Game::inCheckmate(Board b, bool white) {
    // determine if white is in check to begin w, can't be mated without being in check
    if(inCheck(b, white) && generateValidMoves(b, white).size() == 0) {
        return true;
    }
    return false;
}

vector<int> Game::findKing(Board b, bool white) {
    int kingRow = -1;
    int kingCol = -1;
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            if(!b.getCell(row, col).getIsEmpty() 
             && b.getCell(row, col).getPiece()->getPieceType() == PieceType::King 
             && b.getCell(row, col).getPiece()->getIsWhite() == white) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }   
    }
    return {kingRow, kingCol};
}

bool Game::inStalemate(bool whiteTurn)
{ // if self is in stalemate
    // not stalemate if you're in check
    if (inCheck(board, whiteTurn))
    {
        return false;
    }

    vector<Move> validMoves = generateValidMoves(whiteTurn);
    if (validMoves.size() <= 0)
    {
        return true;
    }
    return false;
}

vector<vector<bool>> Game::getHasMovedStates() {
    vector<vector<bool>> hasMovedStates;
    for(int row = 0; row < 8; ++row) {
        hasMovedStates.emplace_back(vector<bool>(8, false));
        for(int col = 0; col < 8; ++col) {
            if(board.getCell(row, col).getIsEmpty()) {
                continue;
            }
            hasMovedStates[row][col] = (board.getCell(row, col).getPiece()->getHasMoved());
        }
    }
    return hasMovedStates;
}


void Game::setHasMovedStates(vector<vector<bool>> states) {
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            if(board.getCell(row, col).getIsEmpty()) {
                continue;
            }
           board.getCell(row, col).getPiece()->setHasMoved(states[row][col]);
        }
    }
}

void Game::handleGameEnd(bool playing) {
    if(playing) {
        return;
    }
    Board newBoard;
    board = newBoard;
    moveList.clear();
    // when game ends, set whiteToMove to true for next game, setup can update
    whiteToMove = true;
    // allow opportunity to setup board
    isSetup = false;
}

Board Game::getBoard() {
    return board;
}

vector<vector<bool>> Game::getHasMovedStates(Board b) {
    vector<vector<bool>> hasMovedStates;
    for(int row = 0; row < 8; ++row) {
        hasMovedStates.emplace_back(vector<bool>(8, false));
        for(int col = 0; col < 8; ++col) {
            if(b.getCell(row, col).getIsEmpty()) {
                continue;
            }
            hasMovedStates[row][col] = (b.getCell(row, col).getPiece()->getHasMoved());
        }
    }
    return hasMovedStates;
}


void Game::setHasMovedStates(Board b, vector<vector<bool>> states) {
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            if(b.getCell(row, col).getIsEmpty()) {
                continue;
            }
           b.getCell(row, col).getPiece()->setHasMoved(states[row][col]);
        }
    }
}
