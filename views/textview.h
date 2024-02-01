#ifndef __TEXTVIEW_H__
#define __TEXTVIEW_H__
#include <iostream>
#include <vector>
#include "view.h"
#include "../board/board.h"

class TextView : public View
{
  Board board;

public:
  TextView(Board board);
  void display() override;
  void setBoard(Board b) override;
};

#endif
