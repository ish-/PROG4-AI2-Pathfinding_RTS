#include "raylib.h"
#include <vector>
using namespace std;
#include "common/math.hpp"

struct Obstacle {
  Rectangle rect;
};
struct Cell {
  ivec2 pos;
  bool obstacle;
};

class Grid {
public:
  ivec2& size;
  vec2& cellSize;

  Grid (ivec2& size, vec2& cellSize)
    : size(size), cellSize(cellSize) {};

  Cell* at (int& x, int& y);
  Cell* at (vec2& pos);

  vector<Cell*> atRect(Rectangle& rect) {
    vector<Cell*> _cells;
    int toX = rect.x + rect.width + 1;
    int toY = rect.y + rect.height + 1;
    _cells.reserve((toX - rect.x) * (toY - rect.y));

    for (int x = rect.x; x < rect.x + rect.width + 1; x++) {
      for (int y = rect.y; y < rect.y + rect.height + 1; y++) {
        _cells.push_back(at(x, y));
      }
    }
    return _cells;
  }

  vector<Cell*> atRadius (vec2 pos, float radius) {
    vector<Cell*> cells;
    Rectangle radiusRect = { pos.x - radius, pos.y - radius, radius * 2, radius * 2 };
    vector<Cell*> cellsRect = atRect(radiusRect);
    float radiusSqr = radius * radius;
    for (auto* cell : cellsRect) {
      if (Vector2DistanceSqr(pos, cell->pos) < radiusSqr)
        cells.push_back(cell);
    }
  }
};

class IPathfind {
public:
  virtual vec2 getDir (vec2& pos) = 0;
};

class PathfindGrid :
    public Grid,
    public IPathfind
{
public:
  PathfindGrid (ivec2& size, vec2& cellSize)
    : Grid(size, cellSize) {};

  void setObstacles (vector<Obstacle*>& obstacles) {
    for (Obstacle* obstacle : obstacles) {
      Rectangle gridObstacleRect = obstacle->rect / cellSize;
      for (Cell* cell : atRect(gridObstacleRect)) {
        cell->obstacle = true;
      }
    }
  }

  vec2 getDir (vec2& pos) override;
};

template <typename TItem>
class Selection {
public:
  bool active;
  vec2 from;
  vec2 to;
  Rectangle rect;

  vector<TItem*> items;

  Selection (vec2& from);

  void start (vec2& from);
  void update (vec2& to);
  void stop (vec2& to);
};

struct Boid {
  vec2 pos;
  vec2 orderVel;
};

class MoveOrder {
public:
  int id;
  vec2& to;
  Selection<Boid>& selection;
  // PathfindGrid pathfinder;

  MoveOrder (vec2& to, Selection<Boid>& selection)
    : to(to), selection(selection) {};

  void update () {
    for (Boid* boid : selection.items) {
      boid->orderVel = pathfinder.getDir(boid->pos);
    }
  };
};

class MoveOrderManager {
  vector<MoveOrder> orders;

  void move (Selection<Boid>& selection, vec2& to) {
    orders.emplace_back(to, selection);
  }

  void update () {
    for (MoveOrder& order : orders) {
      order.update();
    }
  }
};