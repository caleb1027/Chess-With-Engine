#ifndef __VIEW_H__
#define __VIEW_H__

#include "../board/board.h"

class View
{
public:
  virtual void display() = 0;
  virtual ~View() {};
  virtual void setBoard(Board b) = 0;
};

#endif
