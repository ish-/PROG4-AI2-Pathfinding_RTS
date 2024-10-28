#include "Grid.h"
#include "raylib.h"

#include <stdexcept>
#include <iostream>

static int cellsCount = 0;

Cell::Cell (Vector2 _coord, Vector2 _pos, Vector2 _size) {
  pos = _pos; size = _size; coord = _coord;
  idx = cellsCount++;
}

void Cell::draw() const {
  DrawRectangle(pos.x + offset.x, pos.y + offset.y, size.x, size.y, color);
  DrawRectangleLines(pos.x + offset.x, pos.y + offset.y, size.x, size.y, BLUE);
  DrawText(TextFormat("%i", idx), pos.x + 3, pos.y + 3, 15, RED);
};

Grid::Grid (Rectangle _rect, Vector2 _size) {
  rect = _rect; size = _size;
  float dx = rect.width / size.x;
  float dy = rect.height / size.y;

  for (int y = 0; y < size.y; ++y) {
    for (int x = 0; x < size.x; ++x) {
      Vector2 cellPos {rect.x + (dx * x), rect.y + (dy * y)};
      Vector2 cellSize { dx, dy };
      Cell* cell = new Cell({(float)x, (float)y}, cellPos, cellSize);
      cells.push_back(cell);
    }
  }
};

Cell* Grid::at (int x, int y) const {
  int idx = y * size.x + x;
  idx = clamp((float)idx, 0.f, size.x * size.y - 1.f);
  // cout << "x: " << x << "; y: " << y << "; idx=" << idx << "\n";
  return cells.at(idx);
};

void Grid::draw() const {
  for (const Cell* cell : cells) {
    cell->draw();
  }
};

Grid::~Grid() {
  cout << "Grid destroyed\n";
  for (const Cell* cell : cells) {
    delete cell;
  }
  cells.clear();
}
