#include "graphicsview.h"
#include <sstream>

GraphicsView::GraphicsView(Board b): windowSize {696}, oldBoard {}, newBoard {}, xw {windowSize, windowSize} {
    oldBoard.initBoard();
    newBoard.initBoard();
    drawEmptyBoard();
}

void GraphicsView::drawEmptyBoard() {
    int squareLen = windowSize / 8;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            bool tileIsWhite = ((i + j) % 2) == 0;
            if (tileIsWhite) {
                xw.fillLightRectangle(j * squareLen, i * squareLen, squareLen, squareLen);
            } else {
                xw.fillGreenRectangle(j * squareLen, i * squareLen, squareLen, squareLen);
            }

            addTileLabels(i, j);
        }
    }
}

void GraphicsView::redisplayTile(int boardRow, int boardCol) {
    Cell c = newBoard.getCell(boardRow, boardCol);
    int displayRow = (-1 * boardRow) + 7;
    int displayCol = boardCol;
    int squareLen = windowSize / 8;

    // Add tile colour
    if (c.getIsEmpty()) {
        if (c.getIsWhite()) {
            xw.fillLightRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        } else {
            xw.fillGreenRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        }
    } else {
        bool isKing = (c.getPiece()->getPieceType()) == PieceType::King;
        bool isPieceWhite = c.getPiece()->getIsWhite();
        GameState gs = newBoard.getGameState();
        
        if (isKing && isPieceWhite && (gs == GameState::WhiteInCheck || gs == GameState::WhiteInCheckmate)) {
            xw.fillRedRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        } else if (isKing && (!isPieceWhite) && (gs == GameState::BlackInCheck || gs == GameState::BlackInCheckmate)) {
            xw.fillRedRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        } else if (c.getIsWhite()) {
            xw.fillLightRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        } else {
            xw.fillGreenRectangle(displayCol * squareLen, displayRow * squareLen, squareLen, squareLen);
        }
    }

    // Add tile labels
    addTileLabels(displayRow, displayCol);

    // If Cell is empty, return
    if (c.getIsEmpty()) {
        return;
    }

    // Determine type of piece
    PieceType pt = c.getPiece()->getPieceType();
    char pieceChar;
    if (pt == PieceType::Pawn) {
        pieceChar = 'p';
    } else if (pt == PieceType::Rook) {
        pieceChar = 'r';
    } else if (pt == PieceType::Knight) {
        pieceChar = 'n';
    } else if (pt == PieceType::Bishop) {
        pieceChar = 'b';
    } else if (pt == PieceType::Queen) {
        pieceChar = 'q';
    } else {
        pieceChar = 'k';
    }

    char newPieceChar;
    if (c.getPiece()->getIsWhite()) {
        newPieceChar = pieceChar - 32;
    } else {
        newPieceChar = pieceChar;
    }

    // Add piece to display
    ostringstream oss;
    oss << newPieceChar;
    string strToDisplay = oss.str();

    int horizontalOffset = 35;
    int verticalOffset = 50;
    xw.drawString((displayCol * squareLen) + horizontalOffset,
        (displayRow * squareLen) + verticalOffset, strToDisplay);
}

void GraphicsView::addTileLabels(int displayRow, int displayCol) {
    int squareLen = windowSize / 8;
    if (displayRow == 7) {
        char tileLetter = 'a' + displayCol;
        ostringstream oss;
        oss << tileLetter;

        int letterXOffset = 65;
        int letterYOffset = 80;
        xw.drawSmallString((displayCol * squareLen) + letterXOffset,
            (displayRow * squareLen) + letterYOffset, oss.str());
    }

    if (displayCol == 0) {
        int tileNum = 8 - displayRow;
        ostringstream oss;
        oss << tileNum;

        int numXOffset = 5;
        int numYOffset = 25;
        xw.drawSmallString((displayCol * squareLen) + numXOffset,
            (displayRow * squareLen) + numYOffset, oss.str());
    }
}

void GraphicsView::display() {
    for (int boardRow = 0; boardRow < 8; ++boardRow) {
        for (int boardCol = 0; boardCol < 8; ++boardCol) {
            Cell oldCell = oldBoard.getCell(boardRow, boardCol);
            Cell newCell = newBoard.getCell(boardRow, boardCol);

            if ((oldCell.getIsEmpty() != newCell.getIsEmpty()) ||
                (oldCell.getPiece() != newCell.getPiece()) ||
                ((!newCell.getIsEmpty()) && (newCell.getPiece()->getPieceType() == PieceType::King))) {
                
                redisplayTile(boardRow, boardCol);
            }
        }
    }

    oldBoard = newBoard;
}

void GraphicsView::setBoard(Board b) {
    newBoard = b;
}
