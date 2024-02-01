#ifndef __BOARD_H__
#define __BOARD_H__
#include <vector>
#include "cell.h"
#include "../pieces/move.h"

using namespace std;

enum class GameState
{
  NotPlaying,
  Playing,
  WhiteInCheck,
  BlackInCheck,
  WhiteInCheckmate,
  BlackInCheckmate,
  WhiteResigned,
  BlackResigned,
  Stalemate
};

class Board
{
  vector<vector<Cell>> board;
  GameState gameState;
  bool enPassent;
  bool wasEnPassent;
  bool castle;

public:
  Board();
  void addPiece(Cell cell, shared_ptr<Piece> piece);
  void addPiece(int row, int col, PieceType type, bool isWhite);
  void removePiece(Cell cell);
  void removePiece(int row, int col);
  Cell getCell(int row, int col);
  GameState getGameState();
  void setGameState(GameState state);
  void initBoard();
  void initPieces();
  bool move(Move m);
  bool handlePawnPromotion(Cell c, char type);
  void setEnPassent(bool b);
  bool getWasEnPassent() { return wasEnPassent; }
  void setWasEnPassent(bool b) { wasEnPassent = b; }
  void setCastle(bool b);
};

#endif
