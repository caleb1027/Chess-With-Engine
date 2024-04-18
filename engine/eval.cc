#include "eval.h"

Eval::Eval() {
    try {
        module = torch::jit::load("engine/nn/model.pt");
    } catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
    }
}

int Eval::eval(Bitboards board) {
    // Convert the board to a tensor
    torch::Tensor tensor = torch::zeros({1, 808});
    int counter = 0;

    // Get white boards
    for(int i = 5; i >= 0; i--) {
        for(int row = 7; row >= 0; row--) {
            for(int col = 0; col < 8; col++) {
                int index = row * 8 + col;
                if(board.whiteBoards[i] & (1ULL << index)) {
                    tensor[0][counter] = 1;
                }
                ++counter;
            }
        }
    }

    // Get black boards
      for(int i = 5; i >= 0; i--) {
        for(int row = 7; row >= 0; row--) {
            for(int col = 0; col < 8; col++) {
                int index = row * 8 + col;
                if(board.blackBoards[i] & (1ULL << index)) {
                    tensor[0][counter] = 1;
                }
                ++counter;
            }
        }
    }

    // counter should be at 768
    // half moves, convert to binary
    std::bitset<8> halfMoves(board.halfMovesSinceCapture);
    for(int i = 7; i >= 0; --i) {
        tensor[0][counter] = halfMoves[i] ? 1 : 0;
        ++counter;
    }
    
    // skip next row
    counter += 8;
    // full moves, convert to binary
    std::bitset<8> fullMoves(board.halfMovesTotal / 2);
    for(int i = 7; i >= 0; --i) {
        tensor[0][counter] = fullMoves[i] ? 1 : 0;
        ++counter;
    }

    // position of en passent
    if(board.validEnPassentDestinations) {
        // determine index of en passent
        int dest = __builtin_ctzll(board.validEnPassentDestinations);
        int row = 7 - (dest / 8);
        if(board.whiteToMove) {
            row -= 1;
        } else {
            row += 1;
        }
        int col = dest % 8;
        int index = row * 8 + col;
        std::bitset<8> enPassent(index);
         for(int i = 7; i >= 0; --i) {
            tensor[0][counter] = enPassent[i] ? 1 : 0;
            ++counter;
        }
    } else {
        for(int i = 0; i < 8; ++i) {
            tensor[0][counter] = 1;
            ++counter;
        }
    }
    // 5th row: first 2 bits off, 3rd if en passent avaiable, 4th if black to move, 
    counter += 2;
    tensor[0][counter++] = board.validEnPassentDestinations ? 1 : 0;
    tensor[0][counter++] = board.whiteToMove ? 0 : 1;
    // 5th if white castle kingside, 6th if white castle queenside,
    tensor[0][counter++] = board.whiteCastleK ? 1 : 0;
    tensor[0][counter++] = board.whiteCastleQ ? 1 : 0;
    // 7th if black castle kingside, 8th if black castle queenside
    tensor[0][counter++] = board.blackCastleK ? 1 : 0;
    tensor[0][counter++] = board.blackCastleQ ? 1 : 0;

    return module.forward({tensor}).toTensor().item<float>();
}
