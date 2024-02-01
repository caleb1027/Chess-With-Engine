#include "board.h"
#include "../pieces/pieces.h"
#include "../pieces/pieces.h"
#include <iostream>

using namespace std;

Board::Board(): board {vector<vector<Cell>> {}}, gameState {GameState::NotPlaying}, enPassent{false}, wasEnPassent{false}, castle{false} {}

void Board::addPiece(Cell cell, shared_ptr<Piece> piece)
{
  board[cell.getRow()][cell.getCol()].setPiece(piece);
  board[cell.getRow()][cell.getCol()].setIsEmpty(false);
}

void Board::addPiece(int row, int col, PieceType type, bool isWhite)
{
  if (board.size() == 0) {
    initBoard();
  }

  if(!board[row][col].getIsEmpty()) {
    return;
  }

  shared_ptr<Piece> piece;
  if(type == PieceType::King) {
    piece = std::make_shared<King>(col, row, isWhite);
  } else if(type == PieceType::Queen) {
    piece = std::make_shared<Queen>(col, row, isWhite);
  } else if(type == PieceType::Rook) {
    piece = std::make_shared<Rook>(col, row, isWhite);
  } else if(type == PieceType::Knight) {
    piece = std::make_shared<Knight>(col, row, isWhite);
  } else if(type == PieceType::Bishop) {
    piece = std::make_shared<Bishop>(col, row, isWhite);
  } else if(type == PieceType::Pawn) {
    piece = std::make_shared<Pawn>(col, row, isWhite);
  }

  board[row][col].setPiece(piece);
  board[row][col].setIsEmpty(false);
}

void Board::removePiece(int row, int col)
{
  board[row][col].setPiece(nullptr);
  board[row][col].setIsEmpty(true);
}

void Board::removePiece(Cell cell)
{
  board[cell.getRow()][cell.getCol()].setPiece(nullptr);
  board[cell.getRow()][cell.getCol()].setIsEmpty(true);
}

Cell Board::getCell(int row, int col)
{
  return board[row][col];
}

GameState Board::getGameState() { return gameState; }

void Board::setGameState(GameState state) { gameState = state; }

void Board::initBoard()
{
  int rowSize = 8;
  int colSize = 8;

  for (int row = 0; row < rowSize; row++)
  {
    board.emplace_back(vector<Cell>());
    for (int col = 0; col < colSize; col++)
    {
      bool isWhite = ((row + col) % 2 == 1);
      // all empty for now
      board[row].emplace_back(Cell(row, col, isWhite, true, nullptr));
    }
  }
}

void Board::initPieces()
{
  // TODO: Caleb
  int rowSize = 8;
  int colSize = 8;

  for (int row = 0; row < rowSize; row++)
  {
    for (int col = 0; col < colSize; col++)
    {
      if (row == 0) // white pieces
      {
        if (col == 0 || col == 7) // rooks
        {
          addPiece(row, col, PieceType::Rook, true);
        }
        else if (col == 1 || col == 6) // knights
        {
          addPiece(row, col, PieceType::Knight, true);
        }
        else if (col == 2 || col == 5) // bishops
        {
          addPiece(row, col, PieceType::Bishop, true);
        }
        else if (col == 3) // queen
        {
          addPiece(row, col, PieceType::Queen, true);
        }
        else if (col == 4) // king
        {
          addPiece(row, col, PieceType::King, true);
        }
      }
      else if (row == 1) // pawns
      {
        addPiece(row, col, PieceType::Pawn, true);
      }

      else if (row == 7) // black pieces
      {
        if (col == 0 || col == 7) // rooks
        {
          addPiece(row, col, PieceType::Rook, false);
        }
        else if (col == 1 || col == 6) // knights
        {
          addPiece(row, col, PieceType::Knight, false);
        }
        else if (col == 2 || col == 5) // bishops
        {
          addPiece(row, col, PieceType::Bishop, false);
        }
        else if (col == 3) // queen
        {
          addPiece(row, col, PieceType::Queen, false);
        }
        else if (col == 4) // king
        {
          addPiece(row, col, PieceType::King, false);
        }
      }
      else if (row == 6) // pawns
      {
        addPiece(row, col, PieceType::Pawn, false);
      }
    }
  }
}

bool Board::move(Move m)
{ 
  if(wasEnPassent) setWasEnPassent(false);
  // en passent logic
  if(enPassent && !getCell(m.oldRow, m.oldCol).getIsEmpty() && getCell(m.oldRow, m.oldCol).getPiece()->getPieceType() == PieceType::Pawn) {
    removePiece(board[m.oldRow][m.newCol]);
    addPiece(board[m.newRow][m.newCol], board[m.oldRow][m.oldCol].getPiece());
    board[m.oldRow][m.oldCol].getPiece()->move(m);
    removePiece(board[m.oldRow][m.oldCol]);
    setWasEnPassent(true);
  }
  // castling logic (hard coded, as it can only go one way, undefined for setup)

  else if(castle && !getCell(m.oldRow, m.oldCol).getIsEmpty() && getCell(m.oldRow, m.oldCol).getPiece()->getPieceType() == PieceType::King) {
    // castle left
    if(m.newCol == 0 || m.oldCol > m.newCol) {
      // move king
        addPiece(board[m.oldRow][m.oldCol - 2], board[m.oldRow][m.oldCol].getPiece());
        board[m.oldRow][m.oldCol].getPiece()->move(m);
        removePiece(board[m.oldRow][m.oldCol]);
      // move rook
        addPiece(board[m.newRow][3], board[m.oldRow][0].getPiece());
        board[m.oldRow][0].getPiece()->move(m);
        removePiece(board[m.oldRow][0]);
    // castle right
    } else if(m.newCol == 7 || m.oldCol < m.newCol) {
        addPiece(board[m.oldRow][m.oldCol + 2], board[m.oldRow][m.oldCol].getPiece());
        board[m.oldRow][m.oldCol].getPiece()->move(m);
        removePiece(board[m.oldRow][m.oldCol]);
        // move rook
        addPiece(board[m.oldRow][5], board[m.oldRow][7].getPiece());
        board[m.oldRow][7].getPiece()->move(m); 
        removePiece(board[m.oldRow][7]);
    }
  } else {
    board[m.oldRow][m.oldCol].getPiece()->move(m);
    addPiece(board[m.newRow][m.newCol], board[m.oldRow][m.oldCol].getPiece());
    removePiece(board[m.oldRow][m.oldCol]);
  }
  setCastle(false);
  setEnPassent(false);
  return true;
}

bool Board::handlePawnPromotion(Cell c, char type) {
  bool isWhite = c.getPiece()->getIsWhite();
  if(type == 'Q') {
    removePiece(c.getRow(), c.getCol());
    addPiece(c.getRow(), c.getCol(), PieceType::Queen, isWhite);
    c.getPiece()->setHasMoved(true);
  } else if(type == 'R') {
    removePiece(c.getRow(), c.getCol());
    addPiece(c.getRow(), c.getCol(), PieceType::Rook, isWhite);
    c.getPiece()->setHasMoved(true);
  } else if(type == 'B') {
    removePiece(c.getRow(), c.getCol());
    addPiece(c.getRow(), c.getCol(), PieceType::Bishop, isWhite);
    c.getPiece()->setHasMoved(true);
  } else if(type == 'K') {
    removePiece(c.getRow(), c.getCol());
    addPiece(c.getRow(), c.getCol(), PieceType::Knight, isWhite);
    c.getPiece()->setHasMoved(true);
  } else {
    return false;
  }
  return true;
}


void Board::setCastle(bool b) { castle = b; }
void Board::setEnPassent(bool b) { enPassent = b; }
