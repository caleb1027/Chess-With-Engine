#ifndef __CELL_H__
#define __CELL_H__

#include <memory>
#include "../pieces/piece.h"

using namespace std;

class Cell
{
	const int row;
	const int col;
	const bool isWhite;
	bool isEmpty;
	shared_ptr<Piece> piece;

public:
	Cell(int row, int col, bool isWhite, bool isEmpty, shared_ptr<Piece> piece);
	int getRow() const;
	int getCol() const;
	bool getIsWhite() const;
	bool getIsEmpty() const;
	void setIsEmpty(bool empty);
	shared_ptr<Piece> getPiece() const;
	void setPiece(shared_ptr<Piece> p);
	Cell& operator=(const Cell& other);
};
#endif
