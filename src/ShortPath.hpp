#pragma once
#include <vector>
#include <memory>
using namespace std;
#include <raylib.h>

#include "common/math.hpp"
#include "FlowGrid.hpp"
#include "Obstacle.hpp"
#include "Collisions.hpp"

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
  vector<shared_ptr<GridLine>> lines;

  void calc (vector<FlowCell*> path, vector<Obstacle*> obstacles, vec2 gridSize) {
    vector<shared_ptr<GridLine>> straightLines;
    FlowCell* curCell = path[0];
    for (int i = 1; i < path.size(); i++) {
      FlowCell* nextCell = path[i];
      if (curCell->pfToStart == nextCell->pfToStart)
        continue;

      straightLines.push_back(make_shared<GridLine>(curCell->pos, nextCell->pos));
      curCell = nextCell;
    }

    calcShortestLines(straightLines, obstacles, gridSize);
  }

  void calcShortestLines (vector<shared_ptr<GridLine>>_straightLines, vector<Obstacle*> obstacles, vec2 gridSize) {
    int startIdx = 0;
    auto startLine = _straightLines[startIdx];
    for (int i = _straightLines.size() - 1; i > startIdx; i--) {
      auto line = _straightLines[i];

      if (line.get() == startLine.get()) {
        lines.push_back(line);
        startIdx = i + 1;
        i = _straightLines.size() - 1;
        if (startIdx >= _straightLines.size())
          break;
        startLine.reset(_straightLines[startIdx].get());
        continue;
      }

      bool collision = false;
      for (Obstacle* obstacle : obstacles) {
        vec2 start = startLine->start * gridSize;
        vec2 end = line->end * gridSize;

        collision = Collisions::lineRect(start, end, obstacle->rect);
        if (collision)
          break;
      }
      if (collision)
        continue;
      else {
        lines.push_back(make_shared<GridLine>(startLine->start, line->end));
        startIdx = i + 1;
        i = _straightLines.size() - 1;
        if (startIdx >= _straightLines.size())
          break;
        startLine.reset(_straightLines[startIdx].get());
      }
    }
  }

  void draw () {
    for (auto& line : lines)
      line->draw();
  }
};
