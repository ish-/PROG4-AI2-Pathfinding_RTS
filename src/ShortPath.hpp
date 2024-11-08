#pragma once
#include <vector>
#include <memory>
using namespace std;
#include <raylib.h>

#include "common/math.hpp"
#include "FlowGrid.hpp"
#include "Obstacle.hpp"

class GridLine {
public:
  ivec2 start;
  ivec2 end;

  GridLine (ivec2& start, ivec2& end, Color color = WHITE) : start(start), end(end) {

  }

  void draw (vec2 gridSize = FlowGrid::CELL_SIZE) {
    DrawLineEx(
      (vec2){start.x * gridSize.x, start.y * gridSize.y} + gridSize / 2,
      (vec2){end.x * gridSize.x, end.y * gridSize.y} + gridSize / 2,
      2, WHITE);
  }
};

class ShortPath {
public:
  vector<unique_ptr<GridLine>> lines;

  void calc (vector<FlowCell*> path, vector<Obstacle*> obstacles) {
    FlowCell* curCell = path[0];
    for (int i = 1; i < path.size(); i++) {
      FlowCell* nextCell = path[i];
      if (curCell->pfToStart == nextCell->pfToStart)
        continue;

      lines.push_back(make_unique<GridLine>(curCell->pos, nextCell->pos));
      curCell = nextCell;
    }
  }

  void draw () {
    for (auto& line : lines)
      line->draw();
  }
};
