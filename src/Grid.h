#pragma once

#include <vector>
#include "raylib.h"
#include "raymath.h"

using namespace std;

class Cell {
public:
  int idx;
  Color color = WHITE;
  Vector2 size = Vector2{1,1};
  Vector2 offset = Vector2{0,0};
  Vector2 pos = Vector2{0,0};
  Vector2 coord = Vector2{0,0};

  Cell (Vector2 _coord, Vector2 _pos, Vector2 _size);

  void draw() const;
};

class Grid {
public:
  Rectangle rect;
  Vector2 size;
  vector<Cell*> cells;
  Color cellColor = WHITE;

  Grid (Rectangle _rect, Vector2 _size);
  ~Grid ();

  Cell* at (int x, int y) const;

  void draw() const;
};
