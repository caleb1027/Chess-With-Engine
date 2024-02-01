#ifndef __GRAPHICSVIEW_H__
#define __GRAPHICSVIEW_H__

#include "../board/board.h"
#include "view.h"
#include "window.h"

class GraphicsView: public View {
    const int windowSize;
    Board oldBoard;
    Board newBoard;
    Xwindow xw;

    void drawEmptyBoard();
    void redisplayTile(int boardRow, int boardCol);
    void addTileLabels(int displayRow, int displayCol);

    public:
        GraphicsView(Board b);
        void display() override;
        void setBoard(Board b) override;
};

#endif
