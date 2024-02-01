#include <iostream>
#include "textview.h"
using namespace std;

TextView::TextView(Board board) : board(board) {}

void TextView::setBoard(Board b) {
  board = b;
}

void TextView::display()
{
  int colSize = 8;
  string s;

  for (int row = 7; row > -1; --row)
  {
    // row num
    cout << (row + 1) << " ";

    for (int col = 0; col < colSize; ++col)
    {
      if (board.getCell(row, col).getIsEmpty())
      {
        // squares
        (board.getCell(row, col).getIsWhite()) ? s = " " : s = "_";
        cout << s;
        continue;
      }

      PieceType piece = board.getCell(row, col).getPiece()->getPieceType();
      bool isWhite = board.getCell(row, col).getPiece()->getIsWhite();
      // pieces
      if (piece == PieceType::Rook)
      {
        (isWhite) ? s = "R" : s = "r";
      }
      else if (piece == PieceType::Knight)
      {
        (isWhite) ? s = "N" : s = "n";
      }
      else if (piece == PieceType::Bishop)
      {
        (isWhite) ? s = "B" : s = "b";
      }
      else if (piece == PieceType::Queen)
      {
        (isWhite) ? s = "Q" : s = "q";
      }
      else if (piece == PieceType::King)
      {
        (isWhite) ? s = "K" : s = "k";
      }
      else if (piece == PieceType::Pawn)
      {
        (isWhite) ? s = "P" : s = "p";
      }
      cout << s;
    }
    cout << endl;
  }
  cout << endl;
  cout << "  abcdefgh" << endl;
  if (board.getGameState() == GameState::BlackInCheck)
  {
    cout << "Black is in check." << endl;
  }
  else if (board.getGameState() == GameState::WhiteInCheck)
  {
    cout << "White is in check." << endl;
  }
  else if (board.getGameState() == GameState::BlackInCheckmate)
  {
    cout << "Checkmate! White wins!" << endl;
  }
  else if (board.getGameState() == GameState::WhiteInCheckmate)
  {
    cout << "Checkmate! Black wins!" << endl;
  }
  else if (board.getGameState() == GameState::WhiteResigned)
  {
    cout << "Black wins!" << endl;
  }
  else if (board.getGameState() == GameState::BlackResigned)
  {
    cout << "White wins!" << endl;
  }
  else if (board.getGameState() == GameState::Stalemate)
  {
    cout << "Stalemate!" << endl;
  }
}
