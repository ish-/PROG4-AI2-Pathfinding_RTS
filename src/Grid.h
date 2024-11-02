#pragma once

#include <vector>
#include <functional>

#include "raylib.h"
#include "raymath.h"

#include "Cell.h"

using namespace std;

class Grid {
public:
  Rectangle rect;
  Vector2 size;
  vector<Cell*> cells;
  Color cellColor = WHITE;

  Grid (Rectangle _rect, Vector2 _size);
  ~Grid ();

  Cell* at (Vector2 coord) const;
  Cell* at (int x, int y) const;

  void forKernel(Vector2& coord, const std::function<void(Cell* cell, const Vector2& offset)>& func) const;

  void setFlowField(Cell* fromCell) const;

  void draw() const;
};
