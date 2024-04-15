#ifndef __EVAL_H__
#define __EVAL_H__

#include "bitboard.h"
#include <torch/script.h>
#include <bitset>


class Eval {
    private:
        torch::jit::script::Module module;
    public:
        Eval();
        // Given a board, determine the score of the position
        int eval(Bitboards board);
};

#endif
